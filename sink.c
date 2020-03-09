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

#include "sink.h"

void sink_destructor(sink *this)
{
    this->call_tab->destructor(this);
}

size_t sink_source_min(const sink *this)
{
    size_t ret = this->call_tab->source_min(this);
    assert(ret > 0);
    return ret;
}

void sink_send(sink *this)
{
    assert(buffer_read_size(this->source) >= this->call_tab->source_min(this));
    assert(!this->call_tab->end(this));
    this->call_tab->send(this);
}

bool sink_end(const sink *this)
{
    return this->call_tab->end(this);
}

void sink_set_source(buffer *source, sink *this)
{
    assert(source);
    this->source = source;
}
