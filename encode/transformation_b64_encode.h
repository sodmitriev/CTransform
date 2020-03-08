/*!
 * @file transformation_b64_encode.h
 * Declaration of base64 encode transformation class and methods
 */


#ifndef CTRANSFORM_TRANSFORMATION_B64_ENCODE_H
#define CTRANSFORM_TRANSFORMATION_B64_ENCODE_H

#include <transformation.h>

/*!
 * @class transformation_b64_encode
 * @brief A class that implements data base64 encoding transformation
 * @inherit transformation
 */
typedef struct
{
    transformation base;
} transformation_b64_encode;

/*!
 * @fn transformation_b64_encode_constructor(const char *cipher, const char *digest, const char *key,
                                          transformation_b64_encode *this)
 * @brief Construct an base64 encoding transformation
 * @memberof transformation_b64_encode
 * @param this Pointer to "this" object
 */
extern void transformation_b64_encode_constructor(transformation_b64_encode *this);

/*!
 * @fn transformation_b64_encode_destructor(transformation_b64_encode *this)
 * @brief Destructs this transformation
 * @memberof transformation_b64_encode
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_b64_encode_destructor(transformation_b64_encode *this);

/*!
 * @fn transformation_b64_encode_transform(transformation_b64_encode *this)
 * @brief Transform one block of data
 * @memberof transformation_b64_encode
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_b64_encode_transform(transformation_b64_encode *this);

/*!
 * @fn transformation_b64_encode_finalize(transformation_b64_encode *this)
 * @brief Flushes internal buffers
 * @memberof transformation_b64_encode
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_b64_encode_finalize(transformation_b64_encode *this);

/*!
 * @fn transformation_b64_encode_sink_min(const transformation_b64_encode *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_b64_encode
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_b64_encode_sink_min(const transformation_b64_encode *this);

/*!
 * @fn transformation_b64_encode_source_min(const transformation_b64_encode *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_b64_encode
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_b64_encode_source_min(const transformation_b64_encode *this);

#endif //CTRANSFORM_TRANSFORMATION_B64_ENCODE_H
