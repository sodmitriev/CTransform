#ifndef CTRANSFORM_SINK_FILE_H
#define CTRANSFORM_SINK_FILE_H

#include <sink.h>
#include <stdio.h>

typedef struct
{
    sink base;
    FILE* file;
} sink_file;

extern void sink_file_constructor(sink_file *this);

extern void sink_file_destructor(sink_file *this);

extern void sink_file_send(sink_file *this);

extern size_t sink_file_sink_min(sink_file *this);

extern bool sink_file_end(const sink_file *this);

extern void sink_file_open(const char *pathname, sink_file *this);

extern void sink_file_close(sink_file *this);

#endif //CTRANSFORM_SINK_FILE_H
