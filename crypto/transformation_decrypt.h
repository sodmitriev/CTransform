#ifndef CTRANSFORM_TRANSFORMATION_DECRYPT_H
#define CTRANSFORM_TRANSFORMATION_DECRYPT_H

#include <transformation.h>

typedef struct
{
    transformation base;
    struct evp_cipher_ctx_st *ctx;
    size_t block_size;
} transformation_decrypt;

extern void transformation_decrypt_constructor(const char *cipher, const char *digest, const char *key,
                                               transformation_decrypt *this);

extern void transformation_decrypt_destructor(transformation_decrypt *this);

extern void transformation_decrypt_transform(transformation_decrypt *this);

extern bool transformation_decrypt_finalize(transformation_decrypt *this);

extern size_t transformation_decrypt_sink_min(const transformation_decrypt *this);

extern size_t transformation_decrypt_source_min(const transformation_decrypt *this);

#endif //CTRANSFORM_TRANSFORMATION_DECRYPT_H
