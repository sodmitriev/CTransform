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

#include "transformation_replace.h"
#include <assert.h>
#include <string.h>
#include <limits.h>

transformation_call_tab transformation_call_tab_replace =
        {
                .destructor = (void (*)(transformation *))transformation_replace_destructor,
                .transform = (void (*)(transformation *))transformation_replace_transform,
                .finalize = (bool (*)(transformation *))transformation_replace_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_replace_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_replace_source_min
        };

void transformation_replace_transform(transformation_replace *this)
{
    char byte = *buffer_rpos(this->base.source);
    buffer_rinc(1, this->base.source);
    if(this->mask[(unsigned char)byte / 8] & (1u << (unsigned char)((unsigned char)byte % 8)))
    {
        *buffer_wpos(this->base.sink) = this->replace;
    }
    else
    {
        *buffer_wpos(this->base.sink) = byte;
    }
    buffer_winc(1, this->base.sink);
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool transformation_replace_finalize(transformation_replace *this)
{
    return true;
}

size_t transformation_replace_sink_min(const transformation_replace *this)
{
    return 1;
}

size_t transformation_replace_source_min(const transformation_replace *this)
{
    return 1;
}

void transformation_replace_destructor(transformation_replace *this)
{
}

#pragma GCC diagnostic pop

void transformation_replace_constructor(const char *symbols, size_t size, char replace, transformation_replace *this)
{
    assert(sizeof(this->mask) * 8 > UCHAR_MAX);
    memset(&this->mask, 0, sizeof(this->mask));
    for(size_t i = 0; i < size; ++i)
    {
        this->mask[(unsigned char)symbols[i] / 8] |= (uint8_t)(1u << (unsigned char)((unsigned char)symbols[i] % 8));
    }
    this->replace = replace;
    this->base.call_tab = &transformation_call_tab_replace;
}
