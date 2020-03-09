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

#include "transformation_b64_decode.h"
#include <string.h>
#include <openssl/evp.h>
#include <CEasyException/exception.h>
#include <errno.h>

transformation_call_tab transformation_call_tab_b64_decode =
        {
                .destructor = (void (*)(transformation *))transformation_b64_decode_destructor,
                .transform = (void (*)(transformation *))transformation_b64_decode_transform,
                .finalize = (bool (*)(transformation *))transformation_b64_decode_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_b64_decode_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_b64_decode_source_min
        };

//Fix padding because OpenSSL doesn't give a damn
static size_t get_padding(const char *ptr, size_t size)
{
    //trailing '=' -> 1 byte of padding
    //trailing '==' -> 2 bytes of padding
    //trailing '===' -> wrong encoding (openssl should fail)
    //trailing '====' -> 3 bytes of padding
    size_t padding = 0;
    for(const char *end = ptr + size - 1;
        end >= ptr && *end != '+' && *end != '/' && (*end < 'a' || *end > 'z') && (*end < 'A' || *end > 'Z'); --end)
    {
        if(*end == '=')
        {
            ++padding;
        }
    }
    //For each 4 '=' there is 3 bytes missing
    return padding - (padding / 4);
}

void transformation_b64_decode_transform(transformation_b64_decode *this)
{
    int res = EVP_DecodeBlock((unsigned char *)buffer_wpos(this->base.sink),
                              (const unsigned char *)buffer_rpos(this->base.source), 64);
    if(res < 0)
    {
        EXCEPTION_THROW(ENOANO, "%s",
                        "Failed to decode block from base64 (may be caused by spaces or new lines in input string)");
        return;
    }
    buffer_winc(res - get_padding(buffer_rpos(this->base.source), 64), this->base.sink);
    buffer_rinc(64, this->base.source);
}

bool transformation_b64_decode_finalize(transformation_b64_decode *this)
{
    size_t left = buffer_read_size(this->base.source);
    if(left > 0)
    {
        int res = EVP_DecodeBlock((unsigned char *)buffer_wpos(this->base.sink),
                                  (const unsigned char *)buffer_rpos(this->base.source), (int)left);
        if(res < 0)
        {
            EXCEPTION_THROW(ENOANO, "%s",
                            "Failed to decode block from base64 (may be caused by spaces or new lines in input string)");
            return false;
        }
        buffer_winc(res - get_padding(buffer_rpos(this->base.source), left), this->base.sink);
        buffer_rinc(left, this->base.source);
    }
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t transformation_b64_decode_sink_min(const transformation_b64_decode *this)
{
    return 48;
}

size_t transformation_b64_decode_source_min(const transformation_b64_decode *this)
{
    return 64;
}

void transformation_b64_decode_destructor(transformation_b64_decode *this)
{
}

#pragma GCC diagnostic pop

void transformation_b64_decode_constructor(transformation_b64_decode *this)
{
    this->base.call_tab = &transformation_call_tab_b64_decode;
}
