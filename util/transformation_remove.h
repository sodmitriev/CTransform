#ifndef CTRANSFORM_TRANSFORMATION_REMOVE_H
#define CTRANSFORM_TRANSFORMATION_REMOVE_H

#include <transformation.h>
#include <stdint.h>

typedef struct
{
    transformation base;
    uint8_t mask[32];
} transformation_remove;

extern void transformation_remove_constructor(const char *symbols, size_t size, transformation_remove *this);

extern void transformation_remove_destructor(transformation_remove *this);

extern void transformation_remove_transform(transformation_remove *this);

extern void transformation_remove_finalize(transformation_remove *this);

extern size_t transformation_remove_sink_min(const transformation_remove *this);

extern size_t transformation_remove_source_min(const transformation_remove *this);

#endif //CTRANSFORM_TRANSFORMATION_REMOVE_H
