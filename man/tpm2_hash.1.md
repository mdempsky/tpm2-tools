% tpm2_hash(1) tpm2-tools | General Commands Manual

# NAME

**tpm2_hash**(1) - Performs a hash operation with the TPM.

# SYNOPSIS

**tpm2_hash** [*OPTIONS*] _FILE_

# DESCRIPTION

**tpm2_hash**(1) - Performs a hash operation on _FILE_ and returns the results. If
_FILE_ is not specified, then data is read from stdin. If the results of the
hash will be used in a signing operation that uses a restricted signing key,
then the ticket returned by this command can indicate that the hash is safe to
sign.

Output defaults to *stdout* and binary format unless otherwise specified via **-o**
and **--hex** options respectively.

# OPTIONS

  * **-C**, **\--hierarchy**=_HIERARCHY_:

    Hierarchy to use for the ticket. Defaults to **o**, **TPM_RH_OWNER**, when
    no value has been specified.
    Supported options are:
      * **o** for **TPM_RH_OWNER**
      * **p** for **TPM_RH_PLATFORM**
      * **e** for **TPM_RH_ENDORSEMENT**
      * **n** for **TPM_RH_NULL**

  * **-g**, **\--hash-algorithm**=_HASH\_ALGORITHM_:

    The hash algorithm to use.
    Algorithms should follow the "formatting standards", see section
    "Algorithm Specifiers".
    Also, see section "Supported Hash Algorithms" for a list of supported hash
    algorithms.

  * **\--hex**

	Convert the output hmac to hex format without a leading "0x".

  * **-o**, **\--output**=_OUT\_FILE_

    Optional file record of the hash result. Defaults to stdout in hex form.

  * **-t**, **\--ticket**=_TICKET\_FILE_

    Optional file record of the ticket result. Defaults to stdout in hex form.

[common options](common/options.md)

[common tcti options](common/tcti.md)

[supported hash algorithms](common/hash.md)

[algorithm specifiers](common/alg.md)

# EXAMPLES

## Hash a file with sha1 hash algorithm and save the hash and ticket to a file
```bash
tpm2_hash -C e -g sha1 -o hash.bin -t ticket.bin data.txt
```

[returns](common/returns.md)

[footer](common/footer.md)
