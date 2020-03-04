#ifndef CTRANSFORM_SINK_H
#define CTRANSFORM_SINK_H

#include "buffer/buffer.h"

typedef struct
{
    const struct sink_call_tab *call_tab;
    buffer* source;
} sink;

typedef struct sink_call_tab
{
    void (*destructor)(sink* this);

    size_t (*source_min)(const sink* this); //minimum source size

    size_t (*send)(sink* this);

    bool (*end)(const sink* this);
} sink_call_tab;

extern void sink_destructor(sink* this);

extern size_t sink_source_min(sink* this);

extern size_t sink_send(sink* this);

extern bool sink_end(const sink* this);

extern void sink_set_source(buffer* source, sink* this);

#endif //CTRANSFORM_SINK_H
