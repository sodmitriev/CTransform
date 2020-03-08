/*!
 * @file buffer.h
 * @brief Buffer class declaration
 */

#ifndef CTRANSFORM_BUFFER_H
#define CTRANSFORM_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

/*!
 * @class buffer
 * @brief Buffer that carries data between two transformations
 *
 * This struct acts as a buffer that should be placed between two adjacent elements of transformation process. It
 * implements a simple contiguous memory management. Buffer may be both written to and read from. Buffer keeps track of
 * two memory cursors: read cursor and write cursor. During read-write cycle cursors are only moved forwards and never
 * backwards. This may lead to memory space exhaustion. To free unused memory space, buffer_compact(buffer *) should be
 * invoked.
 */
typedef struct
{
    //! @cond PRIVATE
    char *buf;
    size_t size;
    size_t rpos;
    size_t wpos;
    //! @endcond
} buffer;

/*!
 * @fn buffer_constructor(size_t size, buffer *this)
 * @brief Construct a buffer
 * @memberof buffer
 * @param size Buffer size in bytes
 * @param this Pointer to buffer location
 * @throws ENOMEM Failed to allocate requested amount of memory space
 */
extern void buffer_constructor(size_t size, buffer *this);

/*!
 * @fn buffer_destructor(buffer *this)
 * @brief Destroy a buffer, freeing all resources owned by it
 * @memberof buffer
 * @param this Pointer to buffer to destroy
 */
extern void buffer_destructor(buffer *this);

/*!
 * @fn buffer_read_size(const buffer *this)
 * @brief Get amount of bytes that can be read from the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return Amount of bytes that can be read from the buffer
 */
extern size_t buffer_read_size(const buffer *this);

/*!
 * @fn buffer_write_size(const buffer *this)
 * @brief Get amount of bytes that can be written to the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return Amount of bytes that can be written to the buffer
 */
extern size_t buffer_write_size(const buffer *this);

/*!
 * @fn buffer_occupied(const buffer *this)
 * @brief Get amount of bytes that are currently stored in the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return Amount of bytes that are currently stored in the buffer
 */
extern size_t buffer_occupied(const buffer *this);

/*!
 * @fn buffer_readable(const buffer *this)
 * @brief Check if buffer can be read from
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return True if buffer can be read from, false otherwise
 */
extern bool buffer_readable(const buffer *this);

/*!
 * @fn buffer_writable(const buffer *this)
 * @brief Check if buffer can be written to
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return True if buffer can be written to, false otherwise
 */
extern bool buffer_writable(const buffer *this);

/*!
 * @fn buffer_reset(buffer *this)
 * @brief Reset buffer read and write cursors to the beginning of allocated memory space
 * @memberof buffer
 * @param this A pointer to "this" buffer
 */
extern void buffer_reset(buffer *this);

/*!
 * @fn buffer_wpos(buffer *this)
 * @brief Get a write cursor of the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return Write cursor of the buffer (pointer to a memory space that is next to write into)
 */
extern char *buffer_wpos(buffer *this);

/*!
 * @fn buffer_winc(size_t off, buffer *this)
 * @brief Advance the write cursor of the buffer
 * @memberof buffer
 * @param off Amount of bytes that write cursor will be advanced
 * @param this Pointer to "this" buffer
 */
extern void buffer_winc(size_t off, buffer *this);

/*!
 * @fn buffer_rpos(buffer *this)
 * @brief Get a read cursor of the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return Read cursor of the buffer (pointer to a memory space that is next to read from)
 */
extern const char *buffer_rpos(const buffer *this);

/*!
 * @fn buffer_rpos_w(buffer *this)
 * @brief Get a read cursor of the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 * @return Non const read cursor of the buffer (pointer to a memory space that is next to read from)
 *
 * This method is required if a pointer is passed to a method that requires a non const pointer as it's argument.
 * Writing to the memory pointed by the returned pointer will corrupt data in the buffer.
 */
extern char *buffer_rpos_w(buffer *this);

/*!
 * @fn buffer_rinc(size_t off, buffer *this)
 * @brief Advance the read cursor of the buffer
 * @memberof buffer
 * @param off Amount of bytes that read cursor will be advanced
 * @param this Pointer to "this" buffer
 */
extern void buffer_rinc(size_t off, buffer *this);

/*!
 * @fn buffer_size(const buffer *this)
 * @brief Get amount of memory allocated for the buffer
 * @memberof buffer
 * @param this Pointer to "this" buffer
 * @return Amount of memory allocated for the buffer
 */
extern size_t buffer_size(const buffer *this);

/*!
 * @fn buffer_resize(size_t size, buffer *this)
 * @brief Change amount of space allocated for the buffer
 * @memberof buffer
 * @param size New buffer size
 * @param this Pointer to "this" buffer
 *
 * If decrease of buffer size causes read or write cursor to point after the allocated memory space, cursor is replaced
 * to the current end of the buffer, causing a data loss. To avoid this, check buffer_occupied(const buffer *) before
 * invoking this method.
 */
extern void buffer_resize(size_t size, buffer *this);

/*!
 * @fn buffer_compact(buffer *this)
 * @brief Remove processed portion of data from the buffer
 * @memberof buffer
 * @param this A pointer to "this" buffer
 *
 * Invoking this method allows additional data to be written into the buffer, by removing data that was already
 * processed.
 *
 * This method removes all data that is located between buffer beginning and read cursor (i.e. the data that is already
 * read), and moves usable data to the beginning of the buffer, while adjusting read and write cursors.
 */
extern void buffer_compact(buffer *this);

#endif //CTRANSFORM_BUFFER_H
