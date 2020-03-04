#ifndef CTRANSFORM_SOURCE_MEM_H
#define CTRANSFORM_SOURCE_MEM_H

#include <source.h>

typedef struct
{
    source base;
    const void* ptr;
    size_t size;
    size_t nmemb;
    size_t cnt;
} source_write;

extern void source_write_constructor(const void *ptr, size_t size, size_t nmemb, source_write* this);

extern void source_write_destructor(source_write* this);

extern size_t source_write_send(source_write* this);

extern size_t source_write_sink_min(source_write* this);

extern bool source_write_end(const source_write* this);

extern size_t source_write_get_result(const source_write* this);

#endif //CTRANSFORM_SOURCE_MEM_H
