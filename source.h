#ifndef CTRANSFORM_SOURCE_H
#define CTRANSFORM_SOURCE_H

#include "buffer/buffer.h"

typedef struct
{
    const struct source_call_tab *call_tab;
    buffer* sink;
} source;

typedef struct source_call_tab
{
    void (*destructor)(source* this);

    size_t (*sink_min)(const source* this); //minimum sink size

    size_t (*send)(source* this);

    bool (*end)(const source* this);
} source_call_tab;

extern void source_destructor(source* this);

extern size_t source_sink_min(source* this);

extern size_t source_send(source* this);

extern bool source_end(const source* this);

extern void source_set_sink(buffer* sink, source* this);

#endif //CTRANSFORM_SOURCE_H
