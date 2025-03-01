/* SPDX-License-Identifier: BSD-3-Clause */

#ifndef LIB_TPM2_H_
#define LIB_TPM2_H_

#include <tss2/tss2_esys.h>

#include "object.h"
#include "tool_rc.h"

tool_rc tpm2_from_tpm_public(ESYS_CONTEXT *esys_context, TPM2_HANDLE tpm_handle,
        ESYS_TR optional_session1, ESYS_TR optional_session2,
        ESYS_TR optional_session3, ESYS_TR *object);

tool_rc tpm2_close(ESYS_CONTEXT *esys_context, ESYS_TR *rsrc_handle);

tool_rc tpm2_tr_deserialize(ESYS_CONTEXT *esys_context, uint8_t const *buffer,
        size_t buffer_size, ESYS_TR *esys_handle);

tool_rc tpm2_tr_serialize(ESYS_CONTEXT *esys_context, ESYS_TR object,
        uint8_t **buffer, size_t *buffer_size);

tool_rc tpm2_nv_readpublic(ESYS_CONTEXT *esys_context, ESYS_TR nv_index,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        TPM2B_NV_PUBLIC **nv_public, TPM2B_NAME **nv_name);

tool_rc tpm2_readpublic(ESYS_CONTEXT *esys_context, ESYS_TR object_handle,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        TPM2B_PUBLIC **out_public, TPM2B_NAME **name,
        TPM2B_NAME **qualified_name);

tool_rc tpm2_getcap(ESYS_CONTEXT *esys_context, ESYS_TR shandle1,
        ESYS_TR shandle2, ESYS_TR shandle3, TPM2_CAP capability,
        UINT32 property, UINT32 property_count, TPMI_YES_NO *more_data,
        TPMS_CAPABILITY_DATA **capability_data);

tool_rc tpm2_nv_read(ESYS_CONTEXT *esys_context, ESYS_TR auth_handle,
        ESYS_TR nv_index, ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        UINT16 size, UINT16 offset, TPM2B_MAX_NV_BUFFER **data);

tool_rc tpm2_context_save(ESYS_CONTEXT *esys_context, ESYS_TR save_handle,
        TPMS_CONTEXT **context);

tool_rc tpm2_context_load(ESYS_CONTEXT *esys_context,
        const TPMS_CONTEXT *context, ESYS_TR *loaded_handle);

tool_rc tpm2_flush_context(ESYS_CONTEXT *esys_context, ESYS_TR flush_handle);

tool_rc tpm2_start_auth_session(ESYS_CONTEXT *esys_context, ESYS_TR tpm_key,
        ESYS_TR bind, ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        const TPM2B_NONCE *nonce_caller, TPM2_SE session_type,
        const TPMT_SYM_DEF *symmetric, TPMI_ALG_HASH auth_hash,
        ESYS_TR *session_handle);

tool_rc tpm2_sess_set_attributes(ESYS_CONTEXT *esys_context, ESYS_TR session,
        TPMA_SESSION flags, TPMA_SESSION mask);

tool_rc tpm2_sess_get_attributes(ESYS_CONTEXT *esys_context, ESYS_TR session,
        TPMA_SESSION *flags);

tool_rc tpm2_policy_restart(ESYS_CONTEXT *esys_context, ESYS_TR session_handle,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3);

tool_rc tpm2_get_capability(ESYS_CONTEXT *esys_context, ESYS_TR shandle1,
        ESYS_TR shandle2, ESYS_TR shandle3, TPM2_CAP capability,
        UINT32 property, UINT32 property_count, TPMI_YES_NO *more_data,
        TPMS_CAPABILITY_DATA **capability_data);

tool_rc tpm2_create_primary(ESYS_CONTEXT *esys_context, ESYS_TR primary_handle,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        const TPM2B_SENSITIVE_CREATE *in_sensitive, const TPM2B_PUBLIC *in_public,
        const TPM2B_DATA *outside_info, const TPML_PCR_SELECTION *creation_pcr,
        ESYS_TR *object_handle, TPM2B_PUBLIC **out_public,
        TPM2B_CREATION_DATA **creation_data, TPM2B_DIGEST **creation_hash,
        TPMT_TK_CREATION **creation_ticket);

tool_rc tpm2_pcr_read(ESYS_CONTEXT *esys_context, ESYS_TR shandle1,
        ESYS_TR shandle2, ESYS_TR shandle3,
        const TPML_PCR_SELECTION *pcr_selection_in, UINT32 *pcr_update_counter,
        TPML_PCR_SELECTION **pcr_selection_out, TPML_DIGEST **pcr_values);

tool_rc tpm2_policy_authorize(ESYS_CONTEXT *esys_context, ESYS_TR policy_session,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        const TPM2B_DIGEST *approved_policy, const TPM2B_NONCE *policy_ref,
        const TPM2B_NAME *key_sign, const TPMT_TK_VERIFIED *check_ticket);

tool_rc tpm2_policy_or(ESYS_CONTEXT *esys_context, ESYS_TR policy_session,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        const TPML_DIGEST *p_hash_list);

tool_rc tpm2_policy_pcr(ESYS_CONTEXT *esys_context, ESYS_TR policy_session,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        const TPM2B_DIGEST *pcr_digest, const TPML_PCR_SELECTION *pcrs);

tool_rc tpm2_policy_password(ESYS_CONTEXT *esys_context, ESYS_TR policy_session,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3);

tool_rc tpm2_policy_secret(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_entity_obj, ESYS_TR policy_session);

tool_rc tpm2_policy_getdigest(ESYS_CONTEXT *esys_context, ESYS_TR policy_session,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        TPM2B_DIGEST **policy_digest);

tool_rc tpm2_policy_command_code(ESYS_CONTEXT *esys_context,
        ESYS_TR policy_session, ESYS_TR shandle1, ESYS_TR shandle2,
        ESYS_TR shandle3, TPM2_CC code);

tool_rc tpm2_policy_locality(ESYS_CONTEXT *esys_context, ESYS_TR policy_session,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        TPMA_LOCALITY locality);

tool_rc tpm2_policy_duplication_select(ESYS_CONTEXT *esys_context,
        ESYS_TR policy_session, ESYS_TR shandle1, ESYS_TR shandle2,
        ESYS_TR shandle3, const TPM2B_NAME *object_name,
        const TPM2B_NAME *new_parent_name, TPMI_YES_NO include_object);

tool_rc tpm2_tr_get_name(ESYS_CONTEXT *esys_context, ESYS_TR handle,
        TPM2B_NAME **name);

tool_rc tpm2_mu_tpm2_handle_unmarshal(uint8_t const buffer[], size_t size,
        size_t *offset, TPM2_HANDLE *out);

tool_rc tpm2_mu_tpmt_public_marshal(TPMT_PUBLIC const *src, uint8_t buffer[],
        size_t buffer_size, size_t *offset);

tool_rc tpm2_evictcontrol(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj,
        tpm2_loaded_object *to_persist_key_obj,
        TPMI_DH_PERSISTENT persistent_handle, ESYS_TR *new_object_handle);

tool_rc tpm2_hash(ESYS_CONTEXT *esys_context, ESYS_TR shandle1, ESYS_TR shandle2,
        ESYS_TR shandle3, const TPM2B_MAX_BUFFER *data, TPMI_ALG_HASH hash_alg,
        TPMI_RH_HIERARCHY hierarchy, TPM2B_DIGEST **out_hash,
        TPMT_TK_HASHCHECK **validation);

tool_rc tpm2_hash_sequence_start(ESYS_CONTEXT *esys_context, ESYS_TR shandle1,
        ESYS_TR shandle2, ESYS_TR shandle3, const TPM2B_AUTH *auth,
        TPMI_ALG_HASH hash_alg, ESYS_TR *sequence_handle);

tool_rc tpm2_sequence_update(ESYS_CONTEXT *esys_context, ESYS_TR sequence_handle,
        ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
        const TPM2B_MAX_BUFFER *buffer);

tool_rc tpm2_sequence_complete(ESYS_CONTEXT *esys_context,
        ESYS_TR sequence_handle, ESYS_TR shandle1, ESYS_TR shandle2,
        ESYS_TR shandle3, const TPM2B_MAX_BUFFER *buffer,
        TPMI_RH_HIERARCHY hierarchy, TPM2B_DIGEST **result,
        TPMT_TK_HASHCHECK **validation);

tool_rc tpm2_tr_set_auth(ESYS_CONTEXT *esys_context, ESYS_TR handle,
        TPM2B_AUTH const *auth_value);

tool_rc tpm2_activatecredential(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *activatehandle, tpm2_loaded_object *keyhandle,
        const TPM2B_ID_OBJECT *credential_blob,
        const TPM2B_ENCRYPTED_SECRET *secret, TPM2B_DIGEST **cert_info);

tool_rc tpm2_create(ESYS_CONTEXT *esys_context, tpm2_loaded_object *parent_obj,
        const TPM2B_SENSITIVE_CREATE *in_sensitive, const TPM2B_PUBLIC *in_public,
        const TPM2B_DATA *outside_info, const TPML_PCR_SELECTION *creation_pcr,
        TPM2B_PRIVATE **out_private, TPM2B_PUBLIC **out_public,
        TPM2B_CREATION_DATA **creation_data, TPM2B_DIGEST **creation_hash,
        TPMT_TK_CREATION **creation_ticket);

tool_rc tpm2_create_loaded(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *parent_obj,
        const TPM2B_SENSITIVE_CREATE *in_sensitive,
        const TPM2B_TEMPLATE *in_public, ESYS_TR *object_handle,
        TPM2B_PRIVATE **out_private, TPM2B_PUBLIC **out_public);

tool_rc tpm2_object_change_auth(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *parent_object, tpm2_loaded_object *object,
        const TPM2B_AUTH *new_auth, TPM2B_PRIVATE **out_private);

tool_rc tpm2_nv_change_auth(ESYS_CONTEXT *esys_context, tpm2_loaded_object *nv,
        const TPM2B_AUTH *new_auth);

tool_rc tpm2_hierarchy_change_auth(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *hierarchy, const TPM2B_AUTH *new_auth);

tool_rc tpm2_certify(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *certifiedkey_obj,
        tpm2_loaded_object *signingkey_obj, TPM2B_DATA *qualifying_data,
        TPMT_SIG_SCHEME *scheme, TPM2B_ATTEST **certify_info,
        TPMT_SIGNATURE **signature);

tool_rc tpm2_rsa_decrypt(ESYS_CONTEXT *esys_context, tpm2_loaded_object *keyobj,
        const TPM2B_PUBLIC_KEY_RSA *cipher_text,
        const TPMT_RSA_DECRYPT *in_scheme, const TPM2B_DATA *label,
        TPM2B_PUBLIC_KEY_RSA **message);

tool_rc tpm2_load(ESYS_CONTEXT *esys_context, tpm2_loaded_object *parentobj,
        const TPM2B_PRIVATE *in_private, const TPM2B_PUBLIC *in_public,
        ESYS_TR *object_handle);

tool_rc tpm2_clear(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy);

tool_rc tpm2_clearcontrol(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy, TPMI_YES_NO disable_clear);

tool_rc tpm2_dictionarylockout(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy,
        bool clear_lockout,
        bool setup_parameters, UINT32 max_tries, UINT32 recovery_time,
        UINT32 lockout_recovery_time);

tool_rc tpm2_duplicate(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *duplicable_key, ESYS_TR new_parent_handle,
        const TPM2B_DATA *in_key, const TPMT_SYM_DEF_OBJECT *sym_alg,
        TPM2B_DATA **out_key, TPM2B_PRIVATE **duplicate,
        TPM2B_ENCRYPTED_SECRET **encrypted_seed);

tool_rc tpm2_encryptdecrypt(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *encryption_key_obj, TPMI_YES_NO decrypt,
        TPMI_ALG_SYM_MODE mode, const TPM2B_IV *iv_in,
        const TPM2B_MAX_BUFFER *input_data, TPM2B_MAX_BUFFER **output_data,
        TPM2B_IV **iv_out, ESYS_TR shandle1, unsigned *version);

tool_rc tpm2_hierarchycontrol(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy, TPMI_RH_ENABLES enable,
        TPMI_YES_NO state);

tool_rc tpm2_hmac(ESYS_CONTEXT *esys_context, tpm2_loaded_object *hmac_key_obj,
        TPMI_ALG_HASH halg, const TPM2B_MAX_BUFFER *input_buffer,
        TPM2B_DIGEST **out_hmac);

tool_rc tpm2_hmac_start(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *hmac_key_obj, TPMI_ALG_HASH halg,
        ESYS_TR *sequence_handle);

tool_rc tpm2_hmac_sequenceupdate(ESYS_CONTEXT *esys_context,
        ESYS_TR sequence_handle, tpm2_loaded_object *hmac_key_obj,
        const TPM2B_MAX_BUFFER *input_buffer);

tool_rc tpm2_hmac_sequencecomplete(ESYS_CONTEXT *esys_context,
        ESYS_TR sequence_handle, tpm2_loaded_object *hmac_key_obj,
        const TPM2B_MAX_BUFFER *input_buffer, TPM2B_DIGEST **result,
        TPMT_TK_HASHCHECK **validation);

tool_rc tpm2_import(ESYS_CONTEXT *esys_context, tpm2_loaded_object *parent_obj,
        const TPM2B_DATA *encryption_key, const TPM2B_PUBLIC *object_public,
        const TPM2B_PRIVATE *duplicate, const TPM2B_ENCRYPTED_SECRET *in_sym_seed,
        const TPMT_SYM_DEF_OBJECT *symmetric_alg, TPM2B_PRIVATE **out_private);

tool_rc tpm2_nv_definespace(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj, const TPM2B_AUTH *auth,
        const TPM2B_NV_PUBLIC *public_info);

tool_rc tpm2_nv_increment(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj, TPM2_HANDLE nv_index);

tool_rc tpm2_nvreadlock(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj, TPM2_HANDLE nv_index);

tool_rc tpm2_nvundefine(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj, TPM2_HANDLE nv_index);

tool_rc tpm2_nvwrite(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj, TPM2_HANDLE nvindex,
        const TPM2B_MAX_NV_BUFFER *data, UINT16 offset);

tool_rc tpm2_pcr_allocate(ESYS_CONTEXT *esys_context,
        tpm2_loaded_object *auth_hierarchy_obj,
        const TPML_PCR_SELECTION *pcr_allocation);

tool_rc tpm2_sign(ESYS_CONTEXT *esys_context, tpm2_loaded_object *signingkey_obj,
        TPM2B_DIGEST *digest, TPMT_SIG_SCHEME *in_scheme,
        TPMT_TK_HASHCHECK *validation, TPMT_SIGNATURE **signature);

tool_rc tpm2_quote(ESYS_CONTEXT *esys_context, tpm2_loaded_object *quote_obj,
        TPMT_SIG_SCHEME *in_scheme, TPM2B_DATA *qualifying_data,
        TPML_PCR_SELECTION *PCRselect, TPM2B_ATTEST **quoted,
        TPMT_SIGNATURE **signature);

tool_rc tpm2_unseal(ESYS_CONTEXT *esys_context, tpm2_loaded_object *sealkey_obj,
        TPM2B_SENSITIVE_DATA **out_data);

#endif /* LIB_TPM2_H_ */
