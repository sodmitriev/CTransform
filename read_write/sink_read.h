/*!
 * @file sink_read.h
 * Declaration of read sink class and methods
 */

#ifndef CTRANSFORM_SINK_READ_H
#define CTRANSFORM_SINK_READ_H

#include <sink.h>

/*!
 * @class sink_read
 * @brief A class that implements read sink
 * @inherit sink
 *
 * This sink implements an interface similar to fread function.
 * It puts data from the stream into the memory space provided by user
 */
typedef struct
{
    //! @cond PRIVATE
    sink base;
    void *ptr;
    size_t size;
    size_t nmemb;
    size_t cnt;
    //! @endcond
} sink_read;

/*!
 * @fn sink_read_constructor(sink_read *this)
 * @brief Construct a read sink
 * @memberof sink_read
 * @param this Pointer to "this" object
 *
 * Initializes a sink that can't fit any data. Use sink_read_set(const void *, size_t, size_t, sink_read *) to space to
 * this sink.
 */
extern void sink_read_constructor(sink_read *this);

/*!
 * @fn sink_read_destructor(sink_read *this)
 * @brief Destructs this sink
 * @memberof sink_read
 * @param this Pointer to "this" object
 *
 * Does NOT free pointer provided by user
 *
 * Overrides @ref sink_destructor(sink *) method.
 */
extern void sink_read_destructor(sink_read *this);

/*!
 * @fn sink_read_send(sink_read *this)
 * @brief Sends data from the source buffer into this sink
 * @memberof sink_read
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_send(sink *) method.
 */
extern void sink_read_send(sink_read *this);

/*!
 * @fn sink_read_sink_min(const sink_read *this)
 * @brief Returns minimum required amount of readable space in source buffer
 * @memberof sink_read
 * @param this Pointer to "this" object
 *
 * Overrides @ref sink_source_min(const sink *) method.
 */
extern size_t sink_read_source_min(const sink_read *this);

/*!
 * @fn sink_read_end(const sink_read *this)
 * @brief Checks if sink is full
 * @memberof sink_read
 * @param this Pointer to "this" object
 * @return True if sink is full, false otherwise
 *
 * Overrides @ref sink_end(sink *) method.
 */
extern bool sink_read_end(const sink_read *this);

/*!
 * @fn sink_read_set(const void *ptr, size_t size, size_t nmemb, sink_read *this)
 * @brief Sets memory space where sink will write acquired data
 * @memberof sink_read
 * @param ptr Pointer to memory space where sink will write acquired data
 * @param size Size of a single element
 * @param nmemb Maximum amount of elements that provided memory space can contain
 * @param this Pointer to "this" object
 *
 * This method implements an interface similar to fread interface. To get the same result that fread would return, use
 * sink_read_get_result(const sink_read *) method.
 */
extern void sink_read_set(void *ptr, size_t size, size_t nmemb, sink_read *this);

/*!
 * @fn sink_read_get_result(const sink_read *this)
 * @brief Get amount of acquired elements
 * @param this Pointer to "this" object
 * @return Amount of acquired elements
 *
 * This method returns the same result that fread would return. To set same arguments as in fread function, use
 * sink_read_set(const void *, size_t, size_t, sink_read *) method.
 */
extern size_t sink_read_get_result(const sink_read *this);

#endif //CTRANSFORM_SINK_READ_H
