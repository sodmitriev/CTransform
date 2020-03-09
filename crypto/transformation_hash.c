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

#include "transformation_hash.h"
#include <openssl/evp.h>
#include <CEasyException/exception.h>
#include <errno.h>
#include <assert.h>

transformation_call_tab transformation_call_tab_hash =
        {
                .destructor = (void (*)(transformation *))transformation_hash_destructor,
                .transform = (void (*)(transformation *))transformation_hash_transform,
                .finalize = (bool (*)(transformation *))transformation_hash_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_hash_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_hash_source_min
        };


void transformation_hash_destructor(transformation_hash *this)
{
    EVP_MD_CTX_free(this->ctx);
    this->ctx = NULL;
}

void transformation_hash_transform(transformation_hash *this)
{
    size_t size = buffer_read_size(this->base.source);
    if(EVP_DigestUpdate(this->ctx, buffer_rpos(this->base.source), size) == 0)
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to calculate hash of next data block");
        return;
    }
    buffer_rinc(size, this->base.source);
}

bool transformation_hash_finalize(transformation_hash *this)
{
    unsigned int len;
    if(EVP_DigestFinal_ex(this->ctx, (unsigned char *)buffer_wpos(this->base.sink), &len) == 0)
    {
        EXCEPTION_THROW(ENOANO, "%s", "Failed to finalize hash calculation");
        return false;
    }
    assert(len > 0);
    assert((size_t)len == this->md_size);
    buffer_winc(this->md_size, this->base.sink);
    return true;
}

size_t transformation_hash_sink_min(const transformation_hash *this)
{
    return this->md_size;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t transformation_hash_source_min(const transformation_hash *this)
{
    return 1;
}

#pragma GCC diagnostic pop

void transformation_hash_constructor(const char *digest, transformation_hash *this)
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    int md_len;

    md = EVP_get_digestbyname(digest);

    if(!md)
    {
        EXCEPTION_THROW(EINVAL, "Digest \"%s\" not found", digest);
        return;
    }
    md_len = EVP_MD_size(md);

    assert(md_len > 0);

    mdctx = EVP_MD_CTX_new();
    if(mdctx == NULL)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    if(EVP_DigestInit_ex(mdctx, md, NULL) == 0)
    {
        EVP_MD_CTX_free(mdctx);
        EXCEPTION_THROW(ENOANO, "%s", "Failed to initialize md");
        return;
    }

    this->ctx = mdctx;
    this->md_size = (size_t)md_len;
    this->base.call_tab = &transformation_call_tab_hash;
}
