#ifndef CTRANSFORM_TRANSFORMATION_B64_DECODE_H
#define CTRANSFORM_TRANSFORMATION_B64_DECODE_H

#include <transformation.h>

/*!
 * @class transformation_b64_decode
 * @brief A class that implements data base64 decoding transformation
 * @inherit transformation
 */
typedef struct
{
    transformation base;
} transformation_b64_decode;

/*!
 * @fn transformation_b64_decode_constructor(const char *cipher, const char *digest, const char *key,
                                          transformation_b64_decode *this)
 * @brief Construct an base64 decoding transformation
 * @memberof transformation_b64_decode
 * @param this Pointer to "this" object
 */
extern void transformation_b64_decode_constructor(transformation_b64_decode *this);

/*!
 * @fn transformation_b64_decode_destructor(transformation_b64_decode *this)
 * @brief Destructs this transformation
 * @memberof transformation_b64_decode
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_b64_decode_destructor(transformation_b64_decode *this);

/*!
 * @fn transformation_b64_decode_transform(transformation_b64_decode *this)
 * @brief Transform one block of data
 * @memberof transformation_b64_decode
 * @param this Pointer to "this" object
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_b64_decode_transform(transformation_b64_decode *this);

/*!
 * @fn transformation_b64_decode_finalize(transformation_b64_decode *this)
 * @brief Flushes internal buffers
 * @memberof transformation_b64_decode
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_b64_decode_finalize(transformation_b64_decode *this);

/*!
 * @fn transformation_b64_decode_sink_min(const transformation_b64_decode *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_b64_decode
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_b64_decode_sink_min(const transformation_b64_decode *this);

/*!
 * @fn transformation_b64_decode_source_min(const transformation_b64_decode *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_b64_decode
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_b64_decode_source_min(const transformation_b64_decode *this);

#endif //CTRANSFORM_TRANSFORMATION_B64_DECODE_H
