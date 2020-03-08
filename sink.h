/*!
 * @file sink.h
 * @brief Sink abstract class and methods declaration
 */

#ifndef CTRANSFORM_SINK_H
#define CTRANSFORM_SINK_H

#include "buffer/buffer.h"

/*!
 * @class sink
 * @brief Base abstract class for data sink classes
 *
 * This struct acts as a base abstract class for a data sink. For an inheritance example refer to
 * <A HREF=sink__derived__example_8h_source.html><B>sink inheritance example</B></A>.
 *
 * This class acts as a data sink in CTransform. It's main role in the process is to transfer chunks of data to an
 * internal or external source. sink_send(sink *this) is invoked by controller to fulfill this purpose.
 * sink_source_min(const sink *) and sink_end(const sink *) are used by controller to retrieve information about this
 * sink's requirements and status.
 *
 * @section Guarantees
 *
 * @ref controller class provides a set of guarantees for this class method invocation order:
 *
 * - source buffer has at least the required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) >= sink_source_min(this)) when sink_send(sink *) method is invoked
 * - sink is not full (i.e. sink_end(const sink *) equals to false) when sink_send(sink *) method is
 * invoked
 *
 * @section Requirements
 *
 * Any derived sink class shall fulfill the following requirements:
 * - @ref call_tab shall be set in a constructor of the derived class
 * - all methods of @ref call_tab shall be set to appropriate values (same return type, same or derived argument types)
 * - destructor method shall free all resources allocated by the derived class
 * - override of sink_source_min(const sink *) shall return a value higher than 0
 * - invocation of any overridden method, except for destructor, shall not affect the value returned by
 * overridden sink_source_min(const sink *)
 * - override of sink_send(sink *this) shall always result in a source buffer read position advancement or an
 * exception being thrown
 * - any overridden methods, except for sink_send(sink *), shall not throw
 *
 * Violation of any of the requirements results in either an unidentified behaviour
 */
typedef struct
{
    /*! @protected
     * A pointer to a virtual method table, must be set by derived class in it's constructor
     */
    const struct sink_call_tab *call_tab;
    //! @private
    buffer *source;
} sink;

/*! @struct sink_call_tab
 *  @brief A virtual method table struct
 *
 *  Each method from the table must be set in objects of this class in order for it to be used as a derived class
 *  virtual method table.
 *
 *  Each method in this table is invoked by a respective function declared in this header. For overloading example refer
 *  to <A HREF=sink__derived__example_8h_source.html><B>sink inheritance example</B></A>.
 */
typedef struct sink_call_tab
{
    void (*destructor)(sink *this);         ///< Pointer to overridden @ref sink_destructor(sink *) method

    size_t (*source_min)(const sink *this); ///< Pointer to overridden @ref sink_source_min(const sink *) method

    void (*send)(sink *this);               ///< Pointer to overridden @ref sink_send(sink *) method

    bool (*end)(const sink *this);          ///< Pointer to overridden @ref sink_end(const sink *) method
} sink_call_tab;

/*!
 * @fn sink_destructor(sink *this)
 * @brief Frees resources acquired by derived class
 * @memberof sink
 * @param this Pointer to "this" sink
 *
 * Override of this method shall free all resources that were allocated in derived class methods. It may set base class
 * member variables to NULL if desired.
 *
 * Overrides of this methods shall not throw.
 */
extern void sink_destructor(sink *this);

/*!
 * @fn sink_source_min(const sink *this)
 * @brief Returns minimum required amount of readable space in source buffer for send method to be invoked
 * @memberof sink
 * @param this Pointer to "this" sink
 * @return Minimum required amount of readable space in source buffer for send method to be invoked
 *
 * @ref controller guarantees that sink_send(sink *) will be invoked only when source buffer has at least the required
 * amount of readable space (i.e. buffer_read_size(&this->base.source) >= sink_source_min(this)).
 *
 * The returned value shall be higher than zero, and must NOT change as a result of any overridden method invocation
 * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
 * method. Value may change as a result of an invocation of any custom method introduced to derived class.
 *
 * Overrides of this methods shall not throw.
 */
extern size_t sink_source_min(const sink *this);

/*!
 * @fn sink_send(sink *this)
 * @brief Receive next chunk of data from the source buffer
 * @memberof sink
 * @param this Pointer to "this" sink
 * @throws ... Anything override may throw
 *
 * @ref controller guarantees that this method will be invoked only when:
 * - source buffer has at least the required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) >= sink_source_min(this)
 * - sink is not full (i.e. sink_end(const sink *this) equals to false)
 *
 * This method invocation shall result in at least one of the following effects:
 * - source buffer read position has advanced (via buffer_rinc(size_t, buffer *) method)
 * - exception is thrown (via CEasyException)
 *
 * Failing to fulfill this requirement may result in an infinite loop inside of controller's work method.
 */
extern void sink_send(sink *this);

/*!
 * @fn sink_end(const sink *this)
 * @brief Check if sink can consume any more data
 * @memberof sink
 * @param this Pointer to "this" sink
 * @return True if sink is full, false otherwise
 *
 * Checks this sink state and returns true if no more data can be consumed, false otherwise.
 *
 * @ref controller guarantees that sink_send(sink *) will be invoked only when sink is not full
 * (i.e. sink_end(const sink *this) equals to false).
 *
 * Overrides of this methods shall not throw.
 */
extern bool sink_end(const sink *this);

/*!
 * @fn sink_set_source(buffer *source, sink *this)
 * @brief Sets source buffer of a sink object
 * @memberof sink
 * @param source Pointer to the source buffer to use as this sink's source
 * @param this Pointer to "this" sink
 *
 * This method is automatically invoked by @ref controller when needed.
 */
extern void sink_set_source(buffer *source, sink *this);

#endif //CTRANSFORM_SINK_H
