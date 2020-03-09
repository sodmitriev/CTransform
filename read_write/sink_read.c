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

#include "sink_read.h"
#include <string.h>

sink_call_tab sink_call_tab_read =
        {
                .destructor  = (void (*)(sink *))sink_read_destructor,
                .source_min  = (size_t (*)(const sink *))sink_read_source_min,
                .send        = (void (*)(sink *))sink_read_send,
                .end         = (bool (*)(const sink *))sink_read_end
        };

void sink_read_constructor(sink_read *this)
{
    this->base.call_tab = &sink_call_tab_read;
    this->base.source = NULL;
}

void sink_read_destructor(sink_read *this)
{
    this->base.source = NULL;
    this->ptr = NULL;
    this->size = 0;
    this->nmemb = 0;
    this->cnt = 0;
}

size_t sink_read_source_min(const sink_read *this)
{
    return this->size;
}

void sink_read_send(sink_read *this)
{
    size_t src_cnt = buffer_read_size(this->base.source) / this->size;
    size_t need_cnt = this->nmemb - this->cnt;
    size_t count = need_cnt < src_cnt ? need_cnt : src_cnt;
    memcpy((char *)this->ptr + this->cnt * this->size, buffer_rpos(this->base.source), count * this->size);
    buffer_rinc(count * this->size, this->base.source);
    this->cnt += count;
}

bool sink_read_end(const sink_read *this)
{
    return this->cnt >= this->nmemb;
}

void sink_read_set(void *ptr, size_t size, size_t nmemb, sink_read *this)
{
    this->ptr = ptr;
    this->size = size;
    this->nmemb = nmemb;
    this->cnt = 0;
}

size_t sink_read_get_result(const sink_read *this)
{
    return this->cnt;
}
