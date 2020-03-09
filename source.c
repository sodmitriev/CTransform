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

#include "source.h"

void source_destructor(source *this)
{
    this->call_tab->destructor(this);
}

size_t source_sink_min(const source *this)
{
    size_t ret = this->call_tab->sink_min(this);
    assert(ret > 0);
    return ret;
}

void source_send(source *this)
{
    assert(buffer_write_size(this->sink) >= this->call_tab->sink_min(this));
    assert(!this->call_tab->end(this));
    this->call_tab->send(this);
}

bool source_end(const source *this)
{
    return this->call_tab->end(this);
}

void source_set_sink(buffer *sink, source *this)
{
    assert(sink);
    this->sink = sink;
}
