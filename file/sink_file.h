/*!
 * @file sink_file.h
 * Declaration of file sink class and methods
 */

#ifndef CTRANSFORM_SINK_FILE_H
#define CTRANSFORM_SINK_FILE_H

#include <sink.h>
#include <stdio.h>

/*!
 * @class sink_file
 * @brief A class that implements file sink
 * @inherit sink
 *
 * This sink writes data to provided file
 */
typedef struct
{
    sink base;
    FILE* file;
} sink_file;

/*!
 * @fn sink_file_constructor(sink_file *this)
 * @brief Construct a file sink
 * @memberof sink_file
 * @param this Pointer to "this" object
 *
 * Initializes a sink in a closed state. To open file use sink_file_open(const char *, sink_file *)
 */
extern void sink_file_constructor(sink_file *this);

/*!
 * @fn sink_file_destructor(sink_file *this)
 * @brief Destructs this sink
 * @memberof sink_file
 * @param this Pointer to "this" object
 *
 * Will close file if not already closed
 *
 * Overrides @ref sink_destructor(sink *) method.
 */
extern void sink_file_destructor(sink_file *this);

/*!
 * @fn sink_file_send(sink_file *this)
 * @brief Sends data from source buffer to file
 * @memberof sink_file
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fwrite (check C programmer's manual for your system)
 *
 * Overrides @ref sink_send(sink *) method.
 */
extern void sink_file_send(sink_file *this);

/*!
 * @fn sink_file_source_min(const sink_file *this)
 * @brief Returns minimum required amount of writable space in source buffer
 * @memberof sink_file
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fread (check C programmer's manual for your system)
 *
 * Overrides @ref sink_source_min(const sink *) method.
 */
extern size_t sink_file_sink_min(const sink_file *this);

/*!
 * @fn sink_file_end(const sink_file *this)
 * @brief Checks if sink is full
 * @memberof sink_file
 * @param this Pointer to "this" object
 * @return True if sink is full, false otherwise
 *
 * Overrides @ref sink_end(sink *) method.
 */
extern bool sink_file_end(const sink_file *this);

/*!
 * @fn sink_file_open(const char *pathname, sink_file *this)
 * @brief Opens file with specified path for this sink
 * @memberof sink_file
 * @param pathname Path to a file to open
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fopen (check C programmer's manual for your system)
 */
extern void sink_file_open(const char *pathname, sink_file *this);

/*!
 * @fn sink_file_close(sink_file *this)
 * @brief Closes file of this sink
 * @memberof sink_file
 * @param this Pointer to "this" object
 * @throws ... Any error returned by fclose (check C programmer's manual for your system)
 *
 * File is closed even when exception is thrown. Do NOT invoke this method again when it fails.
 */
extern void sink_file_close(sink_file *this);

#endif //CTRANSFORM_SINK_FILE_H
