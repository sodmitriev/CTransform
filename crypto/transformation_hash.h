#ifndef CTRANSFORM_TRANSFORMATION_HASH_H
#define CTRANSFORM_TRANSFORMATION_HASH_H

#include <transformation.h>

typedef struct
{
    transformation base;
    struct evp_md_ctx_st* ctx;
    size_t md_size;
} transformation_hash;

extern void transformation_hash_constructor(const char* digest, transformation_hash* this);

extern void transformation_hash_destructor(transformation_hash* this);

extern void transformation_hash_transform(transformation_hash* this);

extern void transformation_hash_finalize(transformation_hash* this);

extern size_t transformation_hash_sink_min(const transformation_hash* this);

extern size_t transformation_hash_source_min(const transformation_hash* this);

#endif //CTRANSFORM_TRANSFORMATION_HASH_H
