#ifndef CTRANSFORM_SINK_READ_H
#define CTRANSFORM_SINK_READ_H

#include <sink.h>

typedef struct
{
    sink base;
    void* ptr;
    size_t size;
    size_t nmemb;
    size_t cnt;
} sink_read;

extern void sink_read_constructor(void *ptr, size_t size, size_t nmemb, sink_read* this);

extern void sink_read_destructor(sink_read* this);

extern size_t sink_read_source_min(const sink_read* this);

extern size_t sink_read_send(sink_read* this);

extern bool sink_read_end(const sink_read* this);

extern size_t sink_read_get_result(const sink_read* this);

#endif //CTRANSFORM_SINK_READ_H
