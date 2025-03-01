/* SPDX-License-Identifier: BSD-3-Clause */

#include <stdbool.h>

#include "files.h"
#include "log.h"
#include "tpm2.h"
#include "tpm2_capability.h"
#include "tpm2_tool.h"

typedef struct tpm_evictcontrol_ctx tpm_evictcontrol_ctx;
struct tpm_evictcontrol_ctx {
    struct {
        const char *ctx_path;
        const char *auth_str;
        tpm2_loaded_object object;
    } auth_hierarchy;

    struct {
        char *ctx_path;
        tpm2_loaded_object object;
    } to_persist_key;

    TPMI_DH_PERSISTENT persist_handle;

    const char *output_arg;

    struct {
        UINT8 p :1;
        UINT8 c :1;
        UINT8 o :1;
    } flags;
};

static tpm_evictcontrol_ctx ctx = {
    .auth_hierarchy.ctx_path="o",
};

static bool on_option(char key, char *value) {

    switch (key) {
    case 'C':
        ctx.auth_hierarchy.ctx_path = value;
        break;
    case 'P':
        ctx.auth_hierarchy.auth_str = value;
        break;
    case 'c':
        ctx.to_persist_key.ctx_path = value;
        ctx.flags.c = 1;
        break;
    case 'o':
        ctx.output_arg = value;
        ctx.flags.o = 1;
        break;
    }

    return true;
}

static bool on_arg(int argc, char *argv[]) {

    if (argc > 1) {
        LOG_ERR("Expected at most one persistent handle, got %d", argc);
        return false;
    }

    const char *value = argv[0];

    bool result = tpm2_util_string_to_uint32(value, &ctx.persist_handle);
    if (!result) {
        LOG_ERR("Could not convert persistent handle to a number, got: \"%s\"",
                value);
        return false;
    }
    ctx.flags.p = 1;

    return true;
}

bool tpm2_tool_onstart(tpm2_options **opts) {

    const struct option topts[] = {
      { "hierarchy",      required_argument, NULL, 'C' },
      { "auth",           required_argument, NULL, 'P' },
      { "object-context", required_argument, NULL, 'c' },
      { "output",         required_argument, NULL, 'o' },
    };

    *opts = tpm2_options_new("C:P:c:o:", ARRAY_LEN(topts), topts, on_option,
            on_arg, 0);

    return *opts != NULL;
}

tool_rc tpm2_tool_onrun(ESYS_CONTEXT *ectx, tpm2_option_flags flags) {

    UNUSED(flags);

    tool_rc rc = tool_rc_general_error;
    bool evicted = false;

    tool_rc tmp_rc = tpm2_util_object_load(ectx, ctx.to_persist_key.ctx_path,
            &ctx.to_persist_key.object, TPM2_HANDLE_ALL_W_NV);
    if (tmp_rc != tool_rc_success) {
        rc = tmp_rc;
        goto out;
    }

    if (ctx.to_persist_key.object.handle >> TPM2_HR_SHIFT
            == TPM2_HT_PERSISTENT) {
        ctx.persist_handle = ctx.to_persist_key.object.handle;
        ctx.flags.p = 1;
    }

    /* If we've been given a handle or context object to persist and not an
     * explicit persistent handle to use, find an available vacant handle in
     * the persistent namespace and use that.
     */
    if (ctx.flags.c && !ctx.flags.p) {
        tmp_rc = tpm2_capability_find_vacant_persistent_handle(ectx,
                &ctx.persist_handle);
        if (tmp_rc != tool_rc_success) {
            rc = tmp_rc;
            goto out;
        }
        /* we searched and found a persistent handle, so mark that peristent handle valid */
        ctx.flags.p = 1;
    }

    rc = tpm2_util_object_load_auth(ectx, ctx.auth_hierarchy.ctx_path,
            ctx.auth_hierarchy.auth_str, &ctx.auth_hierarchy.object, false,
            TPM2_HANDLE_FLAGS_O | TPM2_HANDLE_FLAGS_P);
    if (ctx.flags.o && !ctx.flags.p) {
        LOG_ERR("Cannot specify -o without using a persistent handle");
        goto out;
    }

    ESYS_TR out_tr;
    rc = tpm2_evictcontrol(ectx, &ctx.auth_hierarchy.object,
            &ctx.to_persist_key.object, ctx.persist_handle, &out_tr);
    if (rc != tool_rc_success) {
        goto out;
    }

    /*
     * Only Close a TR object if it's still resident in the TPM.
     * When these handles match, evictcontrol flushed it from the TPM.
     */
    evicted = ctx.to_persist_key.object.handle == ctx.persist_handle;
    tpm2_tool_output("persistent-handle: 0x%x\n", ctx.persist_handle);
    tpm2_tool_output("action: %s\n", evicted ? "evicted" : "persisted");

    if (ctx.output_arg) {
        rc = files_save_ESYS_TR(ectx, out_tr, ctx.output_arg);
    } else {
        rc = tool_rc_success;
    }

out:
    if (!evicted) {
        TSS2_RC rval = Esys_TR_Close(ectx, &out_tr);
        if (rval != TPM2_RC_SUCCESS) {
            LOG_PERR(Esys_TR_Close, rc);
            rc = tool_rc_from_tpm(rval);
        }
    }

    return rc;
}

tool_rc tpm2_tool_onstop(ESYS_CONTEXT *ectx) {
    UNUSED(ectx);

    return tpm2_session_close(&ctx.auth_hierarchy.object.session);
}
