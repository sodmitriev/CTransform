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
 * @file source_write.h
 * Declaration of write source class and methods
 */

#ifndef CTRANSFORM_SOURCE_MEM_H
#define CTRANSFORM_SOURCE_MEM_H

#include <source.h>

/*!
 * @class source_write
 * @brief A class that implements write source
 * @inherit source
 *
 * This source implements an interface similar to fwrite function.
 * It puts data from the memory space provided by user into the stream
 */
typedef struct
{
    //! @cond PRIVATE
    source base;
    const void *ptr;
    size_t size;
    size_t nmemb;
    size_t cnt;
    //! @endcond
} source_write;

/*!
 * @fn source_write_constructor(source_write *this)
 * @brief Construct a write source
 * @memberof source_write
 * @param this Pointer to "this" object
 *
 * Initializes an empty source. Use source_write_set(const void *, size_t, size_t, source_write *) to add data.
 */
extern void source_write_constructor(source_write *this);

/*!
 * @fn source_write_destructor(source_write *this)
 * @brief Destructs this source
 * @memberof source_write
 * @param this Pointer to "this" object
 *
 * Does NOT free pointer provided by user
 *
 * Overrides @ref source_destructor(source *) method.
 */
extern void source_write_destructor(source_write *this);

/*!
 * @fn source_write_send(source_write *this)
 * @brief Sends data from this source to sink buffer
 * @memberof source_write
 * @param this Pointer to "this" object
 *
 * Overrides @ref source_send(source *) method.
 */
extern void source_write_send(source_write *this);

/*!
 * @fn source_write_sink_min(const source_write *this)
 * @brief Returns minimum required amount of writable space in sink buffer
 * @memberof source_write
 * @param this Pointer to "this" object
 *
 * Overrides @ref source_sink_min(const source *) method.
 */
extern size_t source_write_sink_min(const source_write *this);

/*!
 * @fn source_write_end(const source_write *this)
 * @brief Checks if source is empty
 * @memberof source_write
 * @param this Pointer to "this" object
 * @return True if source is empty, false otherwise
 *
 * Overrides @ref source_end(source *) method.
 */
extern bool source_write_end(const source_write *this);

/*!
 * @fn source_write_set(const void *ptr, size_t size, size_t nmemb, source_write *this)
 * @brief Sets data to be provided by the source
 * @memberof source_write
 * @param ptr Pointer to elements that will be provided by the source
 * @param size Size of a single element
 * @param nmemb Amount of elements
 * @param this Pointer to "this" object
 *
 * This method implements an interface similar to fwrite interface. To get the same result that fwrite would return, use
 * source_write_get_result(const source_write *) method.
 */
extern void source_write_set(const void *ptr, size_t size, size_t nmemb, source_write *this);

/*!
 * @fn source_write_get_result(const source_write *this)
 * @brief Get amount of produced elements
 * @param this Pointer to "this" object
 * @return Amount of produced elements
 *
 * This method returns the same result that fwrite would return. To set same arguments as in fwrite function, use
 * source_write_set(const void *, size_t, size_t, source_write *) method.
 */
extern size_t source_write_get_result(const source_write *this);

#endif //CTRANSFORM_SOURCE_MEM_H
