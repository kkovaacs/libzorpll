#ifndef ZORP_CODE_CIPHER_H_INCLUDED
#define ZORP_CODE_CIPHER_H_INCLUDED

#include <zorp/code.h>
#include <openssl/evp.h>

ZCode *z_code_cipher_new(EVP_CIPHER_CTX *cipher_ctx);


#endif
