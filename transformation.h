#ifndef CTRANSFORM_TRANSFORMATION_H
#define CTRANSFORM_TRANSFORMATION_H

#include "buffer/buffer.h"
#include "source.h"

typedef struct
{
    const struct transformation_call_tab *call_tab;
    buffer *source;
    buffer *sink;
} transformation;

typedef struct transformation_call_tab
{
    void (*destructor)(transformation *this);

    void (*transform)(transformation *this);

    void (*finalize)(transformation *this);

    size_t (*sink_min)(const transformation *this); //minimum sink size, shall be constant

    size_t (*source_min)(const transformation *this); //minimum source size, shall be constant
} transformation_call_tab;

extern void transformation_destructor(transformation *this);

extern void transformation_transform(transformation *this);

extern void transformation_finalize(transformation *this);

extern size_t transformation_sink_min(const transformation *this);

extern size_t transformation_source_min(const transformation *this);

extern void transformation_set_source(buffer *source, transformation *this);

extern void transformation_set_sink(buffer *sink, transformation *this);

#endif //CTRANSFORM_TRANSFORMATION_H
