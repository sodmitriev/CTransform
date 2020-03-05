#ifndef CTRANSFORM_SINK_GETC_H
#define CTRANSFORM_SINK_GETC_H

#include <sink.h>

typedef struct
{
    sink base;
    char chr;
    bool received;
} sink_getc;

extern void sink_getc_constructor(sink_getc *this);

extern void sink_getc_destructor(sink_getc *this);

extern size_t sink_getc_source_min(const sink_getc *this);

extern size_t sink_getc_send(sink_getc *this);

extern bool sink_getc_end(const sink_getc *this);

extern void sink_getc_set(sink_getc *this);

extern int sink_getc_get_result(const sink_getc *this);

#endif //CTRANSFORM_SINK_GETC_H
