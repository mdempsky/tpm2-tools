/* SPDX-License-Identifier: BSD-3-Clause */

#include "log.h"
#include "tpm2_options.h"

typedef struct tpm_selftest_ctx tpm_selftest_ctx;

struct tpm_selftest_ctx {
    TPMI_YES_NO fulltest;
};

static tpm_selftest_ctx ctx;

static tool_rc tpm_selftest(ESYS_CONTEXT *ectx) {
    TSS2_RC rval = Esys_SelfTest(ectx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE,
            ctx.fulltest);
    if (rval != TSS2_RC_SUCCESS) {
        LOG_ERR("TPM SelfTest failed !");
        LOG_PERR(Esys_SelfTest, rval);
        return tool_rc_from_tpm(rval);
    }

    return tool_rc_success;
}

static bool on_option(char key, char *value) {

    UNUSED(value);

    switch (key) {
    case 'f':
        ctx.fulltest = TPM2_YES;
        break;
    default:
        LOG_ERR("Invalid option.");
        return false;
    }

    return true;
}

bool tpm2_tool_onstart(tpm2_options **opts) {

    static const struct option topts[] = {
        { "fulltest", no_argument, NULL, 'f' }
    };

    ctx.fulltest = TPM2_NO;

    *opts = tpm2_options_new("f", ARRAY_LEN(topts), topts, on_option, NULL, 0);

    return *opts != NULL;
}

tool_rc tpm2_tool_onrun(ESYS_CONTEXT *ectx, tpm2_option_flags flags) {

    UNUSED(flags);

    return tpm_selftest(ectx);
}
