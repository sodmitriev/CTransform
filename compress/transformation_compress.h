/*!
 * @file transformation_compress.h
 * Declaration of compress transformation class and methods
 */

#ifndef CTRANSFORM_TRANSFORMATION_COMPRESS_H
#define CTRANSFORM_TRANSFORMATION_COMPRESS_H

#include <transformation.h>

/*!
 * @class transformation_compress
 * @brief A class that implements data compression transformation
 * @inherit transformation
 */
typedef struct
{
    //! @cond PRIVATE
    transformation base;
    struct z_stream_s *stream;
    //! @endcond
} transformation_compress;

/*!
 * @fn transformation_compress_constructor(int level, transformation_compress *this)
 * @brief Construct a compression transformation
 * @memberof transformation_compress
 * @param level Compression level
 * @param this Pointer to "this" object
 * @throws ENOMEM Not enough memory to initialize the internal state
 * @throws ENOANO Unexpected zlib error (that is not listed in zlib documentation)
 *
 * Refer to zlib documentation for more info on compression algorithm
 */
extern void transformation_compress_constructor(int level, transformation_compress *this);

/*!
 * @fn transformation_compress_destructor(transformation_compress *this)
 * @brief Destructs this transformation
 * @memberof transformation_compress
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_compress_destructor(transformation_compress *this);

/*!
 * @fn transformation_compress_transform(transformation_compress *this)
 * @brief Transform one block of data
 * @memberof transformation_compress
 * @param this Pointer to "this" object
 * @throws ENOMEM Not enough memory to perform transformation
 * @throws ENOANO Unexpected zlib error (that is not listed in zlib documentation)
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_compress_transform(transformation_compress *this);

/*!
 * @fn transformation_compress_finalize(transformation_compress *this)
 * @brief Flushes internal buffers
 * @memberof transformation_compress
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 * @throws ENOMEM Not enough memory to perform transformation
 * @throws ENOANO Unexpected zlib error (that is not listed in zlib documentation)
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_compress_finalize(transformation_compress *this);

/*!
 * @fn transformation_compress_sink_min(const transformation_compress *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_compress
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_compress_sink_min(const transformation_compress *this);

/*!
 * @fn transformation_compress_source_min(const transformation_compress *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_compress
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_compress_source_min(const transformation_compress *this);

#endif //CTRANSFORM_TRANSFORMATION_COMPRESS_H
