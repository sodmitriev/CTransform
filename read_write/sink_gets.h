#ifndef CTRANSFORM_SINK_GETS_H
#define CTRANSFORM_SINK_GETS_H

#include <sink.h>

typedef struct
{
    sink base;
    char *s;
    size_t size;
    size_t cnt;
    bool finished;
} sink_gets;

extern void sink_gets_constructor(sink_gets *this);

extern void sink_gets_destructor(sink_gets *this);

extern size_t sink_gets_source_min(const sink_gets *this);

extern void sink_gets_send(sink_gets *this);

extern bool sink_gets_end(const sink_gets *this);

extern void sink_gets_set(char *s, int size, sink_gets *this);

extern char * sink_gets_get_result(const sink_gets *this);

#endif //CTRANSFORM_SINK_GETS_H
