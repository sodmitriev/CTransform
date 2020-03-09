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
 * @file transformation_replace.h
 * Declaration of replace transformation class and methods
 */

#ifndef CTRANSFORM_TRANSFORMATION_REPLACE_H
#define CTRANSFORM_TRANSFORMATION_REPLACE_H

#include <transformation.h>
#include <stdint.h>

/*!
 * @class transformation_replace
 * @brief A class that implements byte replacement transformation
 * @inherit transformation
 * 
 * This transformation replaces bytes with undesired value with a byte of different value
 */
typedef struct
{
    //! @cond PRIVATE
    transformation base;
    uint8_t mask[32];
    char replace;
    //! @endcond
} transformation_replace;

/*!
 * @fn transformation_replace_constructor(const char *symbols, size_t size, char replace, transformation_replace *this)
 * @brief Construct an replace transformation
 * @memberof transformation_replace
 * @param symbols Array of byte values that should be replaced
 * @param size Size of symbol array
 * @param replace Byte value to replace undesired values with
 * @param this Pointer to "this" object
 *
 * Symbol array will be converted to a mask, and may be freed after this method finishes
 */
extern void
transformation_replace_constructor(const char *symbols, size_t size, char replace, transformation_replace *this);

/*!
 * @fn transformation_replace_destructor(transformation_replace *this)
 * @brief Destructs this transformation
 * @memberof transformation_replace
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_replace_destructor(transformation_replace *this);

/*!
 * @fn transformation_replace_transform(transformation_replace *this)
 * @brief Transform one block of data
 * @memberof transformation_replace
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_replace_transform(transformation_replace *this);

/*!
 * @fn transformation_replace_finalize(transformation_replace *this)
 * @brief Flushes internal buffers
 * @memberof transformation_replace
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_replace_finalize(transformation_replace *this);

/*!
 * @fn transformation_replace_sink_min(const transformation_replace *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_replace
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_replace_sink_min(const transformation_replace *this);

/*!
 * @fn transformation_replace_source_min(const transformation_replace *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_replace
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_replace_source_min(const transformation_replace *this);

#endif //CTRANSFORM_TRANSFORMATION_REPLACE_H
