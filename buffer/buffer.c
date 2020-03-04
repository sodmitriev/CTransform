#include "buffer.h"
#include <CEasyException/exception.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

void buffer_constructor(size_t size, buffer* this)
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

void buffer_destructor(buffer* this)
{
    free(this->buf);
    this->buf = NULL;
    this->size = 0;
    this->rpos = 0;
    this->wpos = 0;
}

size_t buffer_read_size(const buffer* this)
{
    return this->wpos - this->rpos;
}

size_t buffer_write_size(const buffer* this)
{
    return this->size - this->wpos;
}

bool buffer_readable(const buffer* this)
{
    return buffer_read_size(this) > 0;
}

bool buffer_writable(const buffer* this)
{
    return buffer_write_size(this) > 0;
}

void buffer_reset(buffer* this)
{
    this->rpos = 0;
    this->wpos = 0;
}

char* buffer_wpos(buffer* this)
{
    return this->buf + this->wpos;
}

void buffer_winc(size_t off, buffer* this)
{
    this->wpos += off;
    assert(this->wpos <= this->size);
}

const char* buffer_rpos(const buffer* this)
{
    return this->buf + this->rpos;
}

void buffer_rinc(size_t off, buffer* this)
{
    this->rpos += off;
    assert(this->rpos <= this->wpos);
}

size_t buffer_size(const buffer* this)
{
    return this->size;
}
