% tpm2_rc_decode(1) tpm2-tools | General Commands Manual

# NAME

**tpm2_rc_decode**(1) - Decode TPM2 error codes to a human readable format.

# SYNOPSIS

**tpm2_rc_decode** [*OPTIONS*] _RC\_CODE_

# DESCRIPTION

**tpm2_rc_decode**(1) - Converts an _RC\_CODE_ from the TPM or TSS2 software stack
into human readable errors. Analogous to **strerror**(3), but for the TPM2 stack.

# OPTIONS

This tool takes no tool specific options.

[common options](common/options.md)

# EXAMPLES

```bash
tpm2_rc_decode 0x1d5
tpm:parameter(1):structure is the wrong size
```

[returns](common/returns.md)

[footer](common/footer.md)
