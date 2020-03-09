/*
 * Copyright (C) 2020 Sviatoslav Dmitriev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "transformation_decrypt.h"
#include <CEasyException/exception.h>
#include <openssl/evp.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

static const unsigned char salt[] = {37, 82, 152, 215, 173, 161, 143, 54};

transformation_call_tab transformation_call_tab_decrypt =
        {
                .destructor = (void (*)(transformation *))transformation_decrypt_destructor,
                .transform = (void (*)(transformation *))transformation_decrypt_transform,
                .finalize = (bool (*)(transformation *))transformation_decrypt_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_decrypt_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_decrypt_source_min
        };

void transformation_decrypt_constructor(const char *cipher, const char *digest, const char *key,
                                        transformation_decrypt *this)
{
    const EVP_CIPHER *ciph = EVP_get_cipherbyname(cipher);
    const EVP_MD *md = EVP_get_digestbyname(digest);

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
    assert(key_size > 0);
    const int iv_size = EVP_CIPHER_iv_length(ciph);
    assert(iv_size > 0);
    const int block_size = EVP_CIPHER_block_size(ciph);
    assert(block_size > 0);
    const size_t key_len = strlen(key);
    if(key_len > INT_MAX)
    {
        EXCEPTION_THROW(EINVAL, "Key is too long (max = %d)", INT_MAX);
        return;
    }

    unsigned char *raw_key = malloc((size_t)key_size);
    unsigned char *iv = malloc((size_t)iv_size);
    if(!raw_key || !iv)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        goto transformation_decrypt_constructor_cleanup;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        goto transformation_decrypt_constructor_cleanup;
    }

    if(EVP_BytesToKey(ciph, md, salt, (const unsigned char *)key, (int)key_len, 3, raw_key, iv) != key_size)
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to initialize key");
        goto transformation_decrypt_constructor_cleanup_ctx;
    }

    if(!EVP_DecryptInit_ex(ctx, ciph, NULL, raw_key, iv))
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to initialize cipher");
        goto transformation_decrypt_constructor_cleanup_ctx;
    }

    this->base.call_tab = &transformation_call_tab_decrypt;
    this->ctx = ctx;
    this->block_size = (size_t)block_size;
    free(raw_key);
    free(iv);
    return;

    transformation_decrypt_constructor_cleanup_ctx:
    EVP_CIPHER_CTX_free(ctx);
    transformation_decrypt_constructor_cleanup:
    free(raw_key);
    free(iv);
}

void transformation_decrypt_destructor(transformation_decrypt *this)
{
    EVP_CIPHER_CTX_free(this->ctx);
    this->ctx = NULL;
}

void transformation_decrypt_transform(transformation_decrypt *this)
{
    int len;
    while(buffer_write_size(this->base.sink) > transformation_decrypt_sink_min(this) &&
          buffer_readable(this->base.source))
    {
        size_t num = this->block_size < buffer_read_size(this->base.source) ?
                     this->block_size : buffer_read_size(this->base.source);
        assert(num <= INT_MAX); //Because block should be smaller, as it's derived from int return of openssl
        if(EVP_DecryptUpdate(this->ctx, (unsigned char *)buffer_wpos(this->base.sink), &len,
                             (const unsigned char *)buffer_rpos(this->base.source), (int)num) != 1)
        {
            EXCEPTION_THROW(ENOANO, "%s", "Failed to decrypt block of data");
            return;
        }
        assert(len >= 0); //OpenSSL should guarantee this
        buffer_rinc(num, this->base.source);
        buffer_winc((size_t)len, this->base.sink);
    }
}

bool transformation_decrypt_finalize(transformation_decrypt *this)
{
    int len;
    if(EVP_DecryptFinal_ex(this->ctx, (unsigned char *)buffer_wpos(this->base.sink), &len) != 1)
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to finalize decryption");
        return false;
    }
    assert(len >= 0); //OpenSSL should guarantee this
    buffer_winc((size_t)len, this->base.sink);
    return true;
}

size_t transformation_decrypt_sink_min(const transformation_decrypt *this)
{
    return this->block_size * 2;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t transformation_decrypt_source_min(const transformation_decrypt *this)
{
    return 1;
}

#pragma GCC diagnostic pop
