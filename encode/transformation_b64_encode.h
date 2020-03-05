#ifndef CTRANSFORM_TRANSFORMATION_B64_ENCODE_H
#define CTRANSFORM_TRANSFORMATION_B64_ENCODE_H

#include <transformation.h>

typedef struct
{
    transformation base;
} transformation_b64_encode;

extern void transformation_b64_encode_constructor(transformation_b64_encode *this);

extern void transformation_b64_encode_destructor(transformation_b64_encode *this);

extern void transformation_b64_encode_transform(transformation_b64_encode *this);

extern void transformation_b64_encode_finalize(transformation_b64_encode *this);

extern size_t transformation_b64_encode_sink_min(const transformation_b64_encode *this);

extern size_t transformation_b64_encode_source_min(const transformation_b64_encode *this);

#endif //CTRANSFORM_TRANSFORMATION_B64_ENCODE_H
