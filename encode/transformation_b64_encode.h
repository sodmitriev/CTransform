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
