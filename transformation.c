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

#include "transformation.h"
#include <assert.h>

void transformation_destructor(transformation *this)
{
    this->call_tab->destructor(this);
    this->source = NULL;
    this->sink = NULL;
}

void transformation_transform(transformation *this)
{
    assert(buffer_read_size(this->source) >= this->call_tab->source_min(this));
    assert(buffer_write_size(this->sink) >= this->call_tab->sink_min(this));
    this->call_tab->transform(this);
}

bool transformation_finalize(transformation *this)
{
    assert(buffer_read_size(this->source) < this->call_tab->source_min(this));
    assert(buffer_write_size(this->sink) >= this->call_tab->sink_min(this));
    return this->call_tab->finalize(this);
}

size_t transformation_sink_min(const transformation *this)
{
    size_t ret = this->call_tab->sink_min(this);
    assert(ret > 0);
    return ret;
}

size_t transformation_source_min(const transformation *this)
{
    size_t ret = this->call_tab->source_min(this);
    assert(ret > 0);
    return ret;
}

void transformation_set_source(buffer *source, transformation *this)
{
    assert(source);
    this->source = source;
}

void transformation_set_sink(buffer *sink, transformation *this)
{
    assert(sink);
    this->sink = sink;
}
