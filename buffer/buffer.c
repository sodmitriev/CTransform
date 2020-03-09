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

#include "buffer.h"
#include <CEasyException/exception.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

void buffer_constructor(size_t size, buffer *this)
{
    this->wpos = 0;
    this->rpos = 0;
    this->buf = malloc(size);
    if(!this->buf)
    {
        this->size = 0;
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    this->size = size;
}

void buffer_destructor(buffer *this)
{
    free(this->buf);
    this->buf = NULL;
    this->size = 0;
    this->rpos = 0;
    this->wpos = 0;
}

size_t buffer_read_size(const buffer *this)
{
    return this->wpos - this->rpos;
}

size_t buffer_write_size(const buffer *this)
{
    return this->size - this->wpos;
}

size_t buffer_occupied(const buffer *this)
{
    return this->wpos;
}

bool buffer_readable(const buffer *this)
{
    return buffer_read_size(this) > 0;
}

bool buffer_writable(const buffer *this)
{
    return buffer_write_size(this) > 0;
}

void buffer_reset(buffer *this)
{
    this->rpos = 0;
    this->wpos = 0;
}

char *buffer_wpos(buffer *this)
{
    return this->buf + this->wpos;
}

void buffer_winc(size_t off, buffer *this)
{
    this->wpos += off;
    assert(this->wpos <= this->size);
}

const char *buffer_rpos(const buffer *this)
{
    return this->buf + this->rpos;
}

char *buffer_rpos_w(buffer *this)
{
    return this->buf + this->rpos;
}

void buffer_rinc(size_t off, buffer *this)
{
    this->rpos += off;
    assert(this->rpos <= this->wpos);
}

size_t buffer_size(const buffer *this)
{
    return this->size;
}

void buffer_resize(size_t size, buffer *this)
{
    //Reallocate memory
    char *new_buf = realloc(this->buf, size);
    if(!new_buf)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    this->buf = new_buf;
    //Set new size
    this->size = size;
    //Adjust read and write cursors if they point after the buffer
    if(this->wpos > this->size)
    {
        this->wpos = this->size;
    }
    if(this->rpos > this->wpos)
    {
        this->rpos = this->wpos;
    }
}

void buffer_compact(buffer *this)
{
    //If read cursor in the beginning buffer can't be compacted
    if(this->rpos == 0)
    {
        return;
    }
    //Move usable data to the beginning of the buffer
    memmove(this->buf, this->buf + this->rpos, this->wpos - this->rpos);
    //Adjust cursors
    this->wpos = this->wpos - this->rpos;
    this->rpos = 0;
}
