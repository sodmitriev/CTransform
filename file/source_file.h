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
 * @file source_file.h
 * Declaration of file source class and methods
 */

#ifndef CTRANSFORM_SOURCE_FILE_H
#define CTRANSFORM_SOURCE_FILE_H

#include <source.h>
#include <stdio.h>

/*!
 * @class source_file
 * @brief A class that implements file source
 * @inherit source
 *
 * This source reads data from provided file
 */
typedef struct
{
    //! @cond PRIVATE
    source base;
    FILE* file;
    //! @endcond
} source_file;

/*!
 * @fn source_file_constructor(source_file *this)
 * @brief Construct a file source
 * @memberof source_file
 * @param this Pointer to "this" object
 *
 * Initializes a source in a closed state. To open file use source_file_open(const char *, source_file *)
 */
extern void source_file_constructor(source_file *this);

/*!
 * @fn source_file_destructor(source_file *this)
 * @brief Destructs this source
 * @memberof source_file
 * @param this Pointer to "this" object
 *
 * Will close file if not already closed
 *
 * Overrides @ref source_destructor(source *) method.
 */
extern void source_file_destructor(source_file *this);

/*!
 * @fn source_file_send(source_file *this)
 * @brief Sends data from file to sink buffer
 * @memberof source_file
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fread (check C programmer's manual for your system)
 *
 * Overrides @ref source_send(source *) method.
 */
extern void source_file_send(source_file *this);

/*!
 * @fn source_file_sink_min(const source_file *this)
 * @brief Returns minimum required amount of writable space in sink buffer
 * @memberof source_file
 * @param this Pointer to "this" object
 *
 * Overrides @ref source_sink_min(const source *) method.
 */
extern size_t source_file_sink_min(const source_file *this);

/*!
 * @fn source_file_end(const source_file *this)
 * @brief Checks if source is empty
 * @memberof source_file
 * @param this Pointer to "this" object
 * @return True if source is empty, false otherwise
 *
 * Overrides @ref source_end(source *) method.
 */
extern bool source_file_end(const source_file *this);

/*!
 * @fn source_file_open(const char *pathname, source_file *this)
 * @brief Opens file with specified path for this source
 * @memberof source_file
 * @param pathname Path to a file to open
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fopen (check C programmer's manual for your system)
 */
extern void source_file_open(const char *pathname, source_file *this);

/*!
 * @fn source_file_close(source_file *this)
 * @brief Closes file of this source
 * @memberof source_file
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fclose (check C programmer's manual for your system)
 *
 * File is closed even when exception is thrown. Do NOT invoke this method again when it fails.
 */
extern void source_file_close(source_file *this);

#endif //CTRANSFORM_SOURCE_FILE_H
