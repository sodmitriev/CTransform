#ifndef CTRANSFORM_TRANSFORMATION_REPLACE_H
#define CTRANSFORM_TRANSFORMATION_REPLACE_H

#include <transformation.h>
#include <stdint.h>

typedef struct
{
    transformation base;
    uint8_t mask[32];
    char replace;
} transformation_replace;

extern void
transformation_replace_constructor(const char *symbols, size_t size, char replace, transformation_replace *this);

extern void transformation_replace_destructor(transformation_replace *this);

extern void transformation_replace_transform(transformation_replace *this);

extern bool transformation_replace_finalize(transformation_replace *this);

extern size_t transformation_replace_sink_min(const transformation_replace *this);

extern size_t transformation_replace_source_min(const transformation_replace *this);

#endif //CTRANSFORM_TRANSFORMATION_REPLACE_H
