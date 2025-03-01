# SPDX-License-Identifier: BSD-3-Clause

source helpers.sh

file_primary_key_ctx=prim.ctx
file_input_data=secret.data
file_policy=policy.data
file_unseal_key_pub=sealkey.pub
file_unseal_key_priv=sealkey.priv
file_unseal_key_ctx=sealkey.ctx
file_unseal_key_name=sealkey.name
file_output_data=unsealed.data
file_session_data=session.dat

secret="12345678"

cleanup() {
    rm -f  $file_primary_key_ctx $file_input_data $file_policy $file_unseal_key_pub\
    $file_unseal_key_priv $file_unseal_key_ctx $file_unseal_key_name\
    $file_output_data $file_session_data

    if [ "${1}" != "no-shutdown" ]; then
        shut_down
    fi
}
trap cleanup EXIT

start_up

cleanup "no-shutdown"

echo $secret > $file_input_data

tpm2_clear

tpm2_createprimary -Q -C o -c $file_primary_key_ctx

tpm2_startauthsession -S $file_session_data

tpm2_policycommandcode -S $file_session_data -L $file_policy unseal

tpm2_flushcontext $file_session_data

rm $file_session_data

echo "tpm2_create -C $file_primary_key_ctx -u $file_unseal_key_pub \
  -r $file_unseal_key_priv -L $file_policy -i- <<< $secret"

tpm2_create -C $file_primary_key_ctx -u $file_unseal_key_pub \
  -r $file_unseal_key_priv -L $file_policy -i- <<< $secret

tpm2_load -C $file_primary_key_ctx -u $file_unseal_key_pub \
  -r $file_unseal_key_priv -n $file_unseal_key_name -c $file_unseal_key_ctx


# Ensure unsealing passes with proper policy
tpm2_startauthsession --policy-session -S $file_session_data

tpm2_policycommandcode -S $file_session_data -L $file_policy unseal

tpm2_unseal -p session:$file_session_data -c sealkey.ctx > $file_output_data

tpm2_flushcontext $file_session_data

rm $file_session_data

cmp -s $file_output_data $file_input_data

# Test that other operations fail
if tpm2_encryptdecrypt -o $file_output_data -c $file_unseal_key_ctx $file_input_data; then
    echo "tpm2_policycommandcode: Should have failed!"
    exit 1
else
    true
fi

exit 0
