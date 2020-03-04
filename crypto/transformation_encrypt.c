#include "transformation_encrypt.h"
#include <CEasyException/exception.h>
#include <openssl/evp.h>
#include <errno.h>
#include <string.h>

static const unsigned char salt[] = {37, 82, 152, 215, 173, 161, 143, 54};

transformation_call_tab transformation_call_tab_encrypt =
        {
                .destructor = (void (*)(transformation *)) transformation_encrypt_destructor,
                .transform = (void (*)(transformation *)) transformation_encrypt_transform,
                .finalize = (void (*)(transformation *)) transformation_encrypt_finalize,
                .sink_min = (size_t (*)(const transformation *)) transformation_encrypt_sink_min,
                .source_min = (size_t (*)(const transformation *)) transformation_encrypt_source_min
        };

void transformation_encrypt_constructor(const char* cipher, const char* digest, const char* key,
                                        transformation_encrypt* this)
{
    const EVP_CIPHER* ciph = EVP_get_cipherbyname(cipher);
    const EVP_MD* md = EVP_get_digestbyname(digest);

    if(ciph == NULL)
    {
        EXCEPTION_THROW(EINVAL, "Cipher \"%s\" not found", cipher);
        return;
    }
    if(md == NULL)
    {
        EXCEPTION_THROW(EINVAL, "Digest \"%s\" not found", digest);
        return;
    }

    const int key_size = EVP_CIPHER_key_length(ciph);
    const int iv_size = EVP_CIPHER_iv_length(ciph);
    const size_t block_size = EVP_CIPHER_block_size(ciph);
    const size_t key_len = strlen(key);

    unsigned char* raw_key = malloc(key_size);
    unsigned char* iv = malloc(iv_size);
    if(!raw_key || !iv)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        goto transformation_encrypt_constructor_cleanup;
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        goto transformation_encrypt_constructor_cleanup;
    }

    if (EVP_BytesToKey(ciph, md, salt, key, key_len, 3, raw_key, iv) != key_size)
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to initialize key");
        goto transformation_encrypt_constructor_cleanup_ctx;
    }

    if(!EVP_EncryptInit_ex(ctx, ciph, NULL, raw_key, iv))
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to initialize cipher");
        goto transformation_encrypt_constructor_cleanup_ctx;
    }

    this->base.call_tab = &transformation_call_tab_encrypt;
    this->ctx = ctx;
    this->block_size = block_size;
    free(raw_key);
    free(iv);
    return;

    transformation_encrypt_constructor_cleanup_ctx:
    EVP_CIPHER_CTX_free(ctx);
    transformation_encrypt_constructor_cleanup:
    free(raw_key);
    free(iv);
}

void transformation_encrypt_destructor(transformation_encrypt* this)
{
    EVP_CIPHER_CTX_free(this->ctx);
    this->ctx = NULL;
}

void transformation_encrypt_transform(transformation_encrypt* this)
{
    int len;
    while(buffer_write_size(this->base.sink) > transformation_encrypt_sink_min(this) && buffer_readable(this->base.source))
    {
        size_t num = this->block_size < buffer_read_size(this->base.source) ?
                this->block_size : buffer_read_size(this->base.source);
        if ( EVP_EncryptUpdate(this->ctx, buffer_wpos(this->base.sink), &len, buffer_rpos(this->base.source), num ) != 1)
        {
            EXCEPTION_THROW(ENOANO, "%s", "Failed to encrypt block of data");
            return;
        }
        buffer_rinc(num, this->base.source);
        buffer_winc(len, this->base.sink);
    }
}

void transformation_encrypt_finalize(transformation_encrypt* this)
{
    int len;
    if(EVP_EncryptFinal_ex(this->ctx, buffer_wpos(this->base.sink), &len) != 1)
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to finalize encryption");
        return;
    }
    buffer_winc(len, this->base.sink);
}

size_t transformation_encrypt_sink_min(const transformation_encrypt* this)
{
    return this->block_size*2 - 1;
}

size_t transformation_encrypt_source_min(const transformation_encrypt* this)
{
    return 1;
}
