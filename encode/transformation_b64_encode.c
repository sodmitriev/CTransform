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

#include "transformation_b64_encode.h"
#include <assert.h>
#include <openssl/evp.h>

transformation_call_tab transformation_call_tab_b64_encode =
        {
                .destructor = (void (*)(transformation *))transformation_b64_encode_destructor,
                .transform = (void (*)(transformation *))transformation_b64_encode_transform,
                .finalize = (bool (*)(transformation *))transformation_b64_encode_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_b64_encode_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_b64_encode_source_min
        };

void transformation_b64_encode_transform(transformation_b64_encode *this)
{
#ifndef NDEBUG
    int written =
#endif
    EVP_EncodeBlock((unsigned char *)buffer_wpos(this->base.sink),
                    (const unsigned char *)buffer_rpos(this->base.source), 48);
#ifndef NDEBUG
    assert(written == 64);
#endif
    buffer_rinc(48, this->base.source);
    buffer_winc(64, this->base.sink);
}

bool transformation_b64_encode_finalize(transformation_b64_encode *this)
{
    if(buffer_readable(this->base.source))
    {
        int written = EVP_EncodeBlock((unsigned char *)buffer_wpos(this->base.sink),
                                      (const unsigned char *)buffer_rpos(this->base.source),
                                      (int)buffer_read_size(this->base.source));
        assert(written >= 0); //Openssl should guarantee this
        buffer_rinc(buffer_read_size(this->base.source), this->base.source);
        buffer_winc((size_t)written, this->base.sink);
    }
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t transformation_b64_encode_sink_min(const transformation_b64_encode *this)
{
    //64 bytes of block + '\0' ('\0' will be omitted)
    return 65;
}

size_t transformation_b64_encode_source_min(const transformation_b64_encode *this)
{
    return 48;
}

void transformation_b64_encode_destructor(transformation_b64_encode *this)
{
}

#pragma GCC diagnostic pop

void transformation_b64_encode_constructor(transformation_b64_encode *this)
{
    this->base.call_tab = &transformation_call_tab_b64_encode;
}
