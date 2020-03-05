#ifndef CTRANSFORM_BUFFER_H
#define CTRANSFORM_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    char *buf;
    size_t size;
    size_t rpos;
    size_t wpos;
} buffer;

extern void buffer_constructor(size_t size, buffer *this);

extern void buffer_destructor(buffer *this);

extern size_t buffer_read_size(const buffer *this);

extern size_t buffer_write_size(const buffer *this);

extern size_t buffer_occupied(const buffer *this);

extern bool buffer_readable(const buffer *this);

extern bool buffer_writable(const buffer *this);

extern void buffer_reset(buffer *this);

extern char *buffer_wpos(buffer *this);

extern void buffer_winc(size_t off, buffer *this);

extern const char *buffer_rpos(const buffer *this);

extern void buffer_rinc(size_t off, buffer *this);

extern size_t buffer_size(const buffer *this);

extern void buffer_resize(size_t size, buffer *this);

extern void buffer_compact(buffer *this);

#endif //CTRANSFORM_BUFFER_H
