/*!
 * @file source.h
 * @brief Source abstract class and methods declaration
 */

#ifndef CTRANSFORM_SOURCE_H
#define CTRANSFORM_SOURCE_H

#include "buffer/buffer.h"

/*!
 * @class source
 * @brief Base abstract class for data source classes
 *
 * This struct acts as a base abstract class for a data source. For an inheritance example refer to
 * <A HREF=source__derived__example_8h_source.html><B>source inheritance example</B></A>.
 *
 * This class acts as a data source in CTransform. It's main role in the process is to provide chunks of data from an
 * internal or external source. source_send(source *this) is invoked by controller to fulfill this purpose.
 * source_sink_min(source *) and source_end(const source *) are used by controller to retrieve information about this
 * source's requirements and status.
 *
 * @section Guarantees
 *
 * @ref controller class provides a set of guarantees for this class method invocation order:
 *
 * - sink buffer has at least the required amount of writable space
 * (i.e. buffer_write_size(&this->base.sink) >= source_sink_min(this) when source_send(source *) method is invoked
 * - source is not empty (i.e. source_end(const source *this) equals to false) when source_send(source *) method is
 * invoked
 *
 * @section Requirements
 *
 * Any derived source class shall fulfill the following requirements:
 * - @ref call_tab shall be set in a constructor of the derived class
 * - all methods of @ref call_tab shall be set to appropriate values (same return type, same or derived argument types)
 * - destructor method shall free all resources allocated by the derived class
 * - override of source_sink_min(source *) shall return a value higher than 0
 * - invocation of any virtual method, except for destructor, shall not affect the value returned by
 * overridden source_sink_min(source *)
 * - override of source_send(source *this) shall always result in a sink buffer write position advancement or an
 * exception being thrown
 * - any overridden methods, except for source_send(source *), shall not throw
 *
 * Violation of any of the requirements results in either an unidentified behaviour or an infinite loop inside of
 * controller methods
 */
typedef struct
{
    /*! @protected
     * A pointer to a virtual method table, must be set by derived class in it's constructor
     */
    const struct source_call_tab *call_tab;
    //! @private
    buffer *sink;
} source;

/*! @struct source_call_tab
 *  @brief A virtual method table struct
 *
 *  Each method from the table must be set in objects of this class in order for it to be used as a derived class
 *  virtual method table.
 *
 *  Each method in this table is invoked by a single function declared in this header. For overloading example refer to
 *  <A HREF=source__derived__example_8h_source.html><B>source inheritance example</B></A>.
 */
typedef struct source_call_tab
{
    void (*destructor)(source *this);       ///< Pointer to overridden @ref source_destructor(source *) method

    size_t (*sink_min)(const source *this); ///< Pointer to overridden @ref source_sink_min(source *) method

    void (*send)(source *this);             ///< Pointer to overridden @ref source_send(source *) method

    bool (*end)(const source *this);        ///< Pointer to overridden @ref source_end(const source *) method
} source_call_tab;

/*!
 * @fn source_destructor(source *this)
 * @brief Frees resources aquired by derived class
 * @memberof source
 * @param this Pointer to "this" source
 *
 * Override of this method shall free all resources that were allocated in derived class methods. It may set base class
 * member variables to NULL if desired.
 *
 * Overrides of this methods shall not throw.
 */
extern void source_destructor(source *this);

/*!
 * @fn source_sink_min(source *this)
 * @brief Returns minimum required amount of writable space in sink buffer for send method to be invoked
 * @memberof source
 * @param this Pointer to "this" source
 * @return Minimum required amount of writable space in sink buffer for send method can be invoked
 *
 * @ref controller guarantees that source_send(source *) will be invoked only when sink buffer has at least the required amount
 * of writable space (i.e. buffer_write_size(&this->base.sink) >= source_sink_min(this)).
 *
 * The returned value shall be higher than zero, and must NOT change as a result of any overridden method invocation
 * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
 * method. Value may change as a result of an invocation of any custom method introduced to derived class.
 *
 * Overrides of this methods shall not throw.
 */
extern size_t source_sink_min(source *this);

/*!
 * @fn source_send(source *this)
 * @brief Send next chunk of data to the sink buffer
 * @memberof source
 * @param this Pointer to "this" source
 * @throws ... Anything override may throw
 *
 * @ref controller guarantees that this method will be invoked only when:
 * - sink buffer has at least the required amount of writable space
 * (i.e. buffer_write_size(&this->base.sink) >= source_sink_min(this)
 * - source is not empty (i.e. source_end(const source *this) equals to false)
 *
 * If this method invocation shall result in at least one of the following effects:
 * - sink buffer write position has advanced (via buffer_winc(size_t, buffer *) method)
 * - exception is thrown (via CEasyException)
 *
 * Failing to fulfill this requirement may result in an infinite loop inside of controller's work method.
 */
extern void source_send(source *this);

/*!
 * @fn source_end(const source *this)
 * @brief Check if there is any more data to be produced by source
 * @memberof source
 * @param this Pointer to "this" source
 * @return True if stream is empty, false otherwise
 *
 * Checks this source state and returns true if there is no more data to be produce, false otherwise.
 *
 * @ref controller guarantees that source_send(source *) will be invoked only when source is not empty
 * (i.e. source_end(const source *this) equals to false).
 *
 * Overrides of this methods shall not throw.
 */
extern bool source_end(const source *this);

/*!
 * @fn source_set_sink(buffer *sink, source *this)
 * @brief Sets sink buffer of a source object
 * @memberof source
 * @param sink Pointer to the sink buffer to use as this source's sink
 * @param this Pointer to "this" source
 *
 * This method is automatically invoked by @ref controller when needed.
 */
extern void source_set_sink(buffer *sink, source *this);

#endif //CTRANSFORM_SOURCE_H
