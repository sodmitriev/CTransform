#ifndef CTRANSFORM_TRANSFORMATION_DECOMPRESS_H
#define CTRANSFORM_TRANSFORMATION_DECOMPRESS_H

#include <transformation.h>

typedef struct
{
    transformation base;
    struct z_stream_s *stream;
    bool finished;
} transformation_decompress;

extern void transformation_decompress_constructor(transformation_decompress *this);

extern void transformation_decompress_destructor(transformation_decompress *this);

extern void transformation_decompress_transform(transformation_decompress *this);

extern bool transformation_decompress_finalize(transformation_decompress *this);

extern size_t transformation_decompress_sink_min(const transformation_decompress *this);

extern size_t transformation_decompress_source_min(const transformation_decompress *this);

#endif //CTRANSFORM_TRANSFORMATION_DECOMPRESS_H
