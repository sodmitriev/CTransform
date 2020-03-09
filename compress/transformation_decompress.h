/*
 * Copyright (C) 2020 Sviatoslav Dmitriev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*!
 * @file transformation_decompress.h
 * Declaration of decompress transformation class and methods
 */

#ifndef CTRANSFORM_TRANSFORMATION_DECOMPRESS_H
#define CTRANSFORM_TRANSFORMATION_DECOMPRESS_H

#include <transformation.h>

/*!
 * @class transformation_decompress
 * @brief A class that implements data decompression transformation
 * @inherit transformation
 */
typedef struct
{
    //! @cond PRIVATE
    transformation base;
    struct z_stream_s *stream;
    bool finished;
    //! @endcond
} transformation_decompress;

/*!
 * @fn transformation_decompress_constructor(int level, transformation_decompress *this)
 * @brief Construct a decompression transformation
 * @memberof transformation_decompress
 * @param this Pointer to "this" object
 * @throws ENOMEM Not enough memory to perform transformation
 * @throws ENOANO Unexpected zlib error (that is not listed in zlib documentation)
 *
 * Refer to zlib documentation for more info on decompression algorithm
 */
extern void transformation_decompress_constructor(transformation_decompress *this);

/*!
 * @fn transformation_decompress_destructor(transformation_decompress *this)
 * @brief Destructs this transformation
 * @memberof transformation_decompress
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_decompress_destructor(transformation_decompress *this);

/*!
 * @fn transformation_decompress_transform(transformation_decompress *this)
 * @brief Transform one block of data
 * @memberof transformation_decompress
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_decompress_transform(transformation_decompress *this);

/*!
 * @fn transformation_decompress_finalize(transformation_decompress *this)
 * @brief Flushes internal buffers
 * @memberof transformation_decompress
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_decompress_finalize(transformation_decompress *this);

/*!
 * @fn transformation_decompress_sink_min(const transformation_decompress *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_decompress
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_decompress_sink_min(const transformation_decompress *this);

/*!
 * @fn transformation_decompress_source_min(const transformation_decompress *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_decompress
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_decompress_source_min(const transformation_decompress *this);

#endif //CTRANSFORM_TRANSFORMATION_DECOMPRESS_H
