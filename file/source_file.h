#ifndef CTRANSFORM_SOURCE_FILE_H
#define CTRANSFORM_SOURCE_FILE_H

#include <source.h>
#include <stdio.h>

typedef struct
{
    source base;
    FILE* file;
} source_file;

extern void source_file_constructor(source_file *this);

extern void source_file_destructor(source_file *this);

extern void source_file_send(source_file *this);

extern size_t source_file_sink_min(source_file *this);

extern bool source_file_end(const source_file *this);

extern void source_file_open(const char *pathname, source_file *this);

extern void source_file_close(source_file *this);

#endif //CTRANSFORM_SOURCE_FILE_H
