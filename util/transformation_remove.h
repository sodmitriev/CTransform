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
 * @file transformation_remove.h
 * Declaration of remove transformation class and methods
 */

#ifndef CTRANSFORM_TRANSFORMATION_REMOVE_H
#define CTRANSFORM_TRANSFORMATION_REMOVE_H

#include <transformation.h>
#include <stdint.h>

/*!
 * @class transformation_remove
 * @brief A class that implements data filtering transformation
 * @inherit transformation
 *
 * This transformation filters the data stream, removing bytes with undesired value
 */
typedef struct
{
    //! @cond PRIVATE
    transformation base;
    uint8_t mask[32];
    //! @endcond
} transformation_remove;

/*!
 * @fn transformation_remove_constructor(const char *symbols, size_t size, transformation_remove *this)
 * @brief Construct an remove transformation
 * @memberof transformation_remove
 * @param symbols Array of byte values that should be filtered out
 * @param size Size of symbol array
 * @param this Pointer to "this" object
 *
 * Symbol array will be converted to a mask, and may be freed after this method finishes
 */
extern void transformation_remove_constructor(const char *symbols, size_t size, transformation_remove *this);

/*!
 * @fn transformation_remove_destructor(transformation_remove *this)
 * @brief Destructs this transformation
 * @memberof transformation_remove
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_remove_destructor(transformation_remove *this);

/*!
 * @fn transformation_remove_transform(transformation_remove *this)
 * @brief Transform one block of data
 * @memberof transformation_remove
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_remove_transform(transformation_remove *this);

/*!
 * @fn transformation_remove_finalize(transformation_remove *this)
 * @brief Flushes internal buffers
 * @memberof transformation_remove
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_remove_finalize(transformation_remove *this);

/*!
 * @fn transformation_remove_sink_min(const transformation_remove *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_remove
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_remove_sink_min(const transformation_remove *this);

/*!
 * @fn transformation_remove_source_min(const transformation_remove *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_remove
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_remove_source_min(const transformation_remove *this);

#endif //CTRANSFORM_TRANSFORMATION_REMOVE_H
