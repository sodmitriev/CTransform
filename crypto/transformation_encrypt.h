#ifndef CTRANSFORM_TRANSFORMATION_ENCRYPT_H
#define CTRANSFORM_TRANSFORMATION_ENCRYPT_H

#include "transformation.h"

typedef struct
{
    transformation base;
    struct evp_cipher_ctx_st* ctx;
    size_t block_size;
} transformation_encrypt;

extern void transformation_encrypt_constructor(const char* cipher, const char* digest, const char* key,
                                               transformation_encrypt* this);

extern void transformation_encrypt_destructor(transformation_encrypt* this);

extern void transformation_encrypt_transform(transformation_encrypt* this);

extern void transformation_encrypt_finalize(transformation_encrypt* this);

extern size_t transformation_encrypt_sink_min(const transformation_encrypt* this);

extern size_t transformation_encrypt_source_min(const transformation_encrypt* this);

#endif //CTRANSFORM_TRANSFORMATION_ENCRYPT_H
