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

#include "transformation_compress.h"
#include <CEasyException/exception.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <stdlib.h>

#define DATA_MIN 2048

transformation_call_tab transformation_call_tab_compress =
        {
                .destructor = (void (*)(transformation *))transformation_compress_destructor,
                .transform = (void (*)(transformation *))transformation_compress_transform,
                .finalize = (bool (*)(transformation *))transformation_compress_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_compress_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_compress_source_min
        };

void transformation_compress_constructor(int level, transformation_compress *this)
{

    z_stream *stream = malloc(sizeof(z_stream));
    if(!stream)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }

    stream->zalloc = Z_NULL;
    stream->zfree = Z_NULL;
    stream->opaque = Z_NULL;

    int ret = deflateInit(stream, level);
    if(ret != Z_OK)
    {
        free(stream);
        assert(ret != Z_STREAM_ERROR);
        assert(ret != Z_VERSION_ERROR);
        if(ret == Z_MEM_ERROR)
        {
            EXCEPTION_THROW_NOMSG(ENOMEM);
        }
        else
        {
            EXCEPTION_THROW(ENOANO, "Failed to initialize compression with error \"%s\"", zError(ret));
        }
        return;
    }

    this->base.call_tab = &transformation_call_tab_compress;
    this->base.source = NULL;
    this->base.sink = NULL;
    this->stream = stream;
}

void transformation_compress_destructor(transformation_compress *this)
{
#ifndef NDEBUG
    int err =
#endif
            deflateEnd(this->stream);
#ifndef NDEBUG
    assert(err == Z_OK);
#endif
    free(this->stream);
    this->stream = NULL;
}

static int do_compression(transformation_compress *this, int flush)
{
    size_t avail;
    avail = buffer_read_size(this->base.source);
    if(avail > (uInt)(0) - 1)
    {
        avail = (uInt)(0) - 1;
    }
    this->stream->avail_in = (uInt)avail;
    avail = buffer_write_size(this->base.sink);
    if(avail > (uInt)(0) - 1)
    {
        avail = (uInt)(0) - 1;
    }
    this->stream->avail_out = (uInt)avail;
    this->stream->next_in = (unsigned char *)buffer_rpos_w(this->base.source);
    this->stream->next_out = (unsigned char *)buffer_wpos(this->base.sink);
    int ret = deflate(this->stream, flush);
    buffer_rinc(buffer_read_size(this->base.source) - this->stream->avail_in, this->base.source);
    buffer_winc(buffer_write_size(this->base.sink) - this->stream->avail_out, this->base.sink);
    return ret;
}

void transformation_compress_transform(transformation_compress *this)
{
    int ret = do_compression(this, Z_NO_FLUSH);
    if(ret != Z_OK)
    {
        assert(ret != Z_BUF_ERROR);
        assert(ret != Z_STREAM_ERROR);
        if(ret == Z_MEM_ERROR)
        {
            EXCEPTION_THROW_NOMSG(ENOMEM);
        }
        else
        {
            EXCEPTION_THROW(ENOANO, "Failed to initialize compression with error \"%s\"", zError(ret));
        }
        return;
    }
}

bool transformation_compress_finalize(transformation_compress *this)
{
    int ret = do_compression(this, Z_FINISH);
    if(ret != Z_OK && ret != Z_STREAM_END)
    {
        assert(ret != Z_BUF_ERROR);
        assert(ret != Z_STREAM_ERROR);
        if(ret == Z_MEM_ERROR)
        {
            EXCEPTION_THROW_NOMSG(ENOMEM);
        }
        else
        {
            EXCEPTION_THROW(ENOANO, "Failed to initialize compression with error \"%d\"", ret);
        }
        return false;
    }
    return ret == Z_STREAM_END;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t transformation_compress_sink_min(const transformation_compress *this)
{
    return DATA_MIN;
}

size_t transformation_compress_source_min(const transformation_compress *this)
{
    return DATA_MIN;
}

#pragma GCC diagnostic pop
