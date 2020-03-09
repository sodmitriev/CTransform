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
 * @file sink_getc.h
 * Declaration of getc sink class and methods
 */

#ifndef CTRANSFORM_SINK_GETC_H
#define CTRANSFORM_SINK_GETC_H

#include <sink.h>

/*!
 * @class sink_getc
 * @brief A class that implements getc sink
 * @inherit sink
 *
 * This sink implements an interface similar to fgetc function.
 * It takes one byte from the stream.
 */
typedef struct
{
    //! @cond PRIVATE
    sink base;
    char chr;
    bool received;
    //! @endcond
} sink_getc;

/*!
 * @fn sink_getc_constructor(sink_getc *this)
 * @brief Construct a getc sink
 * @memberof sink_getc
 * @param this Pointer to "this" object
 *
 * Initializes a sink that can't fit any data. Use sink_getc_set(sink_getc *) to make it writable.
 */
extern void sink_getc_constructor(sink_getc *this);

/*!
 * @fn sink_getc_destructor(sink_getc *this)
 * @brief Destructs this sink
 * @memberof sink_getc
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_destructor(sink *) method.
 */
extern void sink_getc_destructor(sink_getc *this);

/*!
 * @fn sink_getc_send(sink_getc *this)
 * @brief Sends data from the source buffer into this sink
 * @memberof sink_getc
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_send(sink *) method.
 */
extern void sink_getc_send(sink_getc *this);

/*!
 * @fn sink_getc_sink_min(const sink_getc *this)
 * @brief Returns minimum required amount of readable space in source buffer
 * @memberof sink_getc
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_source_min(const sink *) method.
 */
extern size_t sink_getc_source_min(const sink_getc *this);


/*!
 * @fn sink_getc_end(const sink_getc *this)
 * @brief Checks if sink is full
 * @memberof sink_getc
 * @param this Pointer to "this" object
 * @return True if sink is full, false otherwise
 *
 * Overrides @ref sink_end(sink *) method.
 */
extern bool sink_getc_end(const sink_getc *this);

/*!
 * @fn sink_getc_set(sink_getc *this)
 * @brief Enables writing into this sink for a single byte
 * @memberof sink_getc
 * @param this Pointer to "this" object
 * 
 * This method implements an interface similar to fgetc interface. To get the same result that fgetc would return, use
 * sink_getc_get_result(const sink_getc *) method.
 */
extern void sink_getc_set(sink_getc *this);

/*!
 * @fn sink_getc_get_result(const sink_getc *this)
 * @brief Get the acquired character
 * @param this Pointer to "this" object
 * @return Acquired character cast to int, or EOF on error
 * 
 * This method returns the same result that fgetc would return. To set same arguments as in fgetc function, use 
 * sink_getc_set(sink_getc *) method.
 */
extern int sink_getc_get_result(const sink_getc *this);

#endif //CTRANSFORM_SINK_GETC_H
