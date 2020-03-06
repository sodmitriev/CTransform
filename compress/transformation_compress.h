#ifndef CTRANSFORM_TRANSFORMATION_COMPRESS_H
#define CTRANSFORM_TRANSFORMATION_COMPRESS_H

#include <transformation.h>

typedef struct
{
    transformation base;
    struct z_stream_s *stream;
} transformation_compress;

extern void transformation_compress_constructor(int level, transformation_compress *this);

extern void transformation_compress_destructor(transformation_compress *this);

extern void transformation_compress_transform(transformation_compress *this);

extern bool transformation_compress_finalize(transformation_compress *this);

extern size_t transformation_compress_sink_min(const transformation_compress *this);

extern size_t transformation_compress_source_min(const transformation_compress *this);

#endif //CTRANSFORM_TRANSFORMATION_COMPRESS_H
