/*!
 * @file sink_gets.h
 * Declaration of gets sink class and methods
 */

#ifndef CTRANSFORM_SINK_GETS_H
#define CTRANSFORM_SINK_GETS_H

#include <sink.h>

/*!
 * @class sink_gets
 * @brief A class that implements gets sink
 * @inherit sink
 *
 * This sink implements an interface similar to fgets function.
 * It reads at most one line from the stream.
 */
typedef struct
{
    //! @cond PRIVATE
    sink base;
    char *s;
    size_t size;
    size_t cnt;
    bool finished;
    //! @endcond
} sink_gets;

/*!
 * @fn sink_gets_constructor(sink_gets *this)
 * @brief Construct a gets sink
 * @memberof sink_gets
 * @param this Pointer to "this" object
 *
 * Initializes a sink that can't fit any data. Use sink_gets_set(char *, int, sink_gets *) to make it writable.
 */
extern void sink_gets_constructor(sink_gets *this);

/*!
 * @fn sink_gets_destructor(sink_gets *this)
 * @brief Destructs this sink
 * @memberof sink_gets
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_destructor(sink *) method.
 */
extern void sink_gets_destructor(sink_gets *this);

/*!
 * @fn sink_gets_send(sink_gets *this)
 * @brief Sends data from the source buffer into this sink
 * @memberof sink_gets
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_send(sink *) method.
 */
extern void sink_gets_send(sink_gets *this);

/*!
 * @fn sink_gets_sink_min(const sink_gets *this)
 * @brief Returns minimum required amount of readable space in source buffer
 * @memberof sink_gets
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_source_min(const sink *) method.
 */
extern size_t sink_gets_source_min(const sink_gets *this);

/*!
 * @fn sink_gets_end(const sink_gets *this)
 * @brief Checks if sink is full
 * @memberof sink_gets
 * @param this Pointer to "this" object
 * @return True if sink is full, false otherwise
 *
 * Overrides @ref sink_end(sink *) method.
 */
extern bool sink_gets_end(const sink_gets *this);

/*!
 * @fn sink_gets_set(sink_gets *this)
 * @brief Sets memory space where sink will write acquired data
 * @memberof sink_gets
 * @param s Pointer to memory space where sink will write acquired data
 * @param size Maximum available space pointed by s in bytes
 * @param this Pointer to "this" object
 *
 * Resulting string will contain new line character if it was encountered, and a terminating null byte at the end of the
 * string
 *
 * This method implements an interface similar to fgets interface. To get the same result that fgets would return, use
 * sink_gets_get_result(const sink_gets *) method.
 */
extern void sink_gets_set(char *s, int size, sink_gets *this);

/*!
 * @fn sink_gets_get_result(const sink_gets *this)
 * @brief Get the line reading result
 * @param this Pointer to "this" object
 * @return Pointer to set string location, or NULL on error
 * 
 * This method returns the same result that fgets would return. To set same arguments as in fgets function, use 
 * sink_gets_set(char *, int, sink_gets *) method.
 */
extern char * sink_gets_get_result(const sink_gets *this);

#endif //CTRANSFORM_SINK_GETS_H
