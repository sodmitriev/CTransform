#ifndef CTRANSFORM_TRANSFORMATION_B64_DECODE_H
#define CTRANSFORM_TRANSFORMATION_B64_DECODE_H

#include <transformation.h>

typedef struct
{
    transformation base;
} transformation_b64_decode;

extern void transformation_b64_decode_constructor(transformation_b64_decode* this);

extern void transformation_b64_decode_destructor(transformation_b64_decode* this);

extern void transformation_b64_decode_transform(transformation_b64_decode* this);

extern void transformation_b64_decode_finalize(transformation_b64_decode* this);

extern size_t transformation_b64_decode_sink_min(const transformation_b64_decode* this);

extern size_t transformation_b64_decode_source_min(const transformation_b64_decode* this);

#endif //CTRANSFORM_TRANSFORMATION_B64_DECODE_H
