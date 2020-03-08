/*!
 * @file transformation.h
 * @brief Transformation abstract class and methods declaration
 */

#ifndef CTRANSFORM_TRANSFORMATION_H
#define CTRANSFORM_TRANSFORMATION_H

#include "buffer/buffer.h"

/*!
 * @class transformation
 * @brief Base abstract class for data transformation classes
 *
 * This struct acts as a base abstract class for a data transformation. For an inheritance example refer to
 * <A HREF=transformation__derived__example_8h_source.html><B>transformation inheritance example</B></A>.
 *
 * This class acts as a data transformation in CTransform. It's main role in the process is to take data from source
 * buffer, transform in a desired way, and put it into the sink buffer. transformation_transform(transformation *this)
 * and transformation_finalize(transformation *this) methods are invoked by controller to fulfill this purpose.
 * transformation_source_min(const transformation *) and transformation_sink_min(const transformation *) are used by
 * controller to retrieve information about this transformation's requirements applied to source and sink buffers
 * respectively.
 *
 * @section Guarantees
 *
 * @ref controller class provides a set of guarantees for this class method invocation order:
 *
 * - source buffer has at least the required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) >= transformation_source_min(this)) when
 * transformation_transform(transformation *) method is invoked
 * - source buffer has at less than required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) < transformation_source_min(this)) when
 * transformation_finalize(transformation *) method is invoked
 * - sink buffer has at least the required amount of writable space
 * (i.e. buffer_write_size(&this->base.sink) >= transformation_sink_min(this)) when
 * transformation_transform(transformation *) and transformation_finalize(transformation *) methods are invoked
 * - transformation_transform(transformation *) is never invoked if transformation_finalize(transformation *) was
 * previously invoked on this transformation
 * - transformation_finalize(transformation *) is never invoked if transformation_finalize(transformation *) was
 * previously invoked on this transformation and returned true
 *
 * @section Requirements
 *
 * Any derived sink class shall fulfill the following requirements:
 * - @ref call_tab shall be set in a constructor of the derived class
 * - all methods of @ref call_tab shall be set to appropriate values (same return type, same or derived argument types)
 * - destructor method shall free all resources allocated by the derived class
 * - override of transformation_source_min(const transformation *) shall return a value higher than 0
 * - override of transformation_sink_min(const transformation *) shall return a value higher than 0
 * - invocation of any overridden method, except for destructor, shall not affect the value returned by
 * overridden transformation_source_min(const transformation *) and transformation_sink_min(const transformation *)
 * methods
 * - override of transformation_transform(transformation *this) shall always result in source buffer read position
 * advancement, sink buffer read position advancement or an exception being thrown
 * - if transformation_finalize(transformation *this) method is infinitely invoked in a loop, while no other
 * transformation methods are invoked, it shall eventually return true
 * - any overridden methods, except for transformation_transform(transformation *) and
 * transformation_finalize(transformation *), shall not throw
 *
 * Violation of any of the requirements results in either an unidentified behaviour.
 */
typedef struct
{
    /*! @protected
     * A pointer to a virtual method table, must be set by derived class in it's constructor
     */
    const struct transformation_call_tab *call_tab;
    //! @private
    buffer *source;
    //! @private
    buffer *sink;
} transformation;

/*! @struct transformation_call_tab
 *  @brief A virtual method table struct
 *
 *  Each method from the table must be set in objects of this class in order for it to be used as a derived class
 *  virtual method table.
 *
 *  Each method in this table is invoked by a respective function declared in this header. For overloading example refer
 *  to <A HREF=transformation__derived__example_8h_source.html><B>transformation inheritance example</B></A>.
 */
typedef struct transformation_call_tab
{
    //! Pointer to overridden @ref transformation_destructor(transformation *) method
    void (*destructor)(transformation *this);

    //! Pointer to overridden @ref transformation_transform(transformation *) method
    void (*transform)(transformation *this);

    //! Pointer to overridden @ref transformation_finalize(transformation *) method
    bool (*finalize)(transformation *this);

    //! Pointer to overridden @ref transformation_sink_min(const transformation *) method
    size_t (*sink_min)(const transformation *this);

    //! Pointer to overridden @ref transformation_source_min(const transformation *) method
    size_t (*source_min)(const transformation *this);
} transformation_call_tab;

/*!
 * @fn transformation_destructor(transformation *this)
 * @brief Frees resources acquired by derived class
 * @memberof transformation
 * @param this Pointer to "this" transformation
 *
 * Override of this method shall free all resources that were allocated in derived class methods. It may set base class
 * member variables to NULL if desired.
 *
 * Overrides of this methods shall not throw.
 */
extern void transformation_destructor(transformation *this);

/*!
 * @fn transformation_transform(transformation *this)
 * @brief Receive next chunk of data from the source buffer, transform it, and sent so the sink buffer
 * @memberof transformation
 * @param this Pointer to "this" transformation
 * @throws ... Anything override may throw
 *
 * @ref controller guarantees that this method will be invoked only when:
 * - source buffer has at least the required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) >= transformation_source_min(this)
 * - sink buffer has at least the required amount of writable space
 * (i.e. buffer_write_size(&this->base.sink) >= transformation_sink_min(this)
 * - transformation_finalize(transformation *) was never invoked on on this object.
 *
 * This method invocation shall result in at least one of the following effects:
 * - source buffer read position has advanced (via buffer_rinc(size_t, buffer *) method)
 * - sink buffer write position has advanced (via buffer_winc(size_t, buffer *) method)
 * - exception is thrown (via CEasyException)
 *
 * Failing to fulfill this requirement may result in an infinite loop inside of controller's work method.
 */
extern void transformation_transform(transformation *this);

/*!
 * @fn transformation_finalize(transformation *this)
 * @brief Flush all data from transformation's internal buffers, and sent so the sink buffer
 * @memberof transformation
 * @param this Pointer to "this" transformation
 * @return true if transformation was successfully finalized, false otherwise
 * @throws ... Anything override may throw
 *
 * @ref controller guarantees that this method will be invoked only when:
 * - source buffer has less than required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) < transformation_source_min(this)
 * - sink buffer has at least the required amount of writable space
 * (i.e. buffer_write_size(&this->base.sink) >= transformation_sink_min(this)
 * - transformation_finalize(transformation *) never returned true after being invoked on on this object.
 *
 * If this method is infinitely invoked in a loop, while no other transformation methods are invoked,
 * it shall eventually return true.
 *
 * Failing to fulfill this requirement may result in an infinite loop inside of controller's work method.
 *
 * This method shall return true if all internal buffers were flushed and no more actions will be produced by this
 * transformation.
 * If this method returns false, it will be invoked again by the controller to continue finalization process.
 * If this method returns true, it will never be invoked again on this transformation.
 */
extern bool transformation_finalize(transformation *this);

/*!
 * @fn transformation_sink_min(const transformation *this)
 * @brief Returns minimum required amount of writable space in sink buffer for transform and finalize methods to
 * be invoked
 * @memberof transformation
 * @param this Pointer to "this" transformation
 * @return Minimum required amount of writable space in sink buffer for transform and finalize methods to be invoked
 *
 * @ref controller guarantees that transformation_transform(const transformation *) and
 * transformation_finalize(const transformation *) methods will be invoked only when sink
 * buffer has at least the required amount of writable space
 * (i.e. buffer_write_size(&this->base.sink) >= transformation_sink_min(this)).
 *
 * The returned value shall be higher than zero, and must NOT change as a result of any overridden method invocation
 * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
 * method. Value may change as a result of an invocation of any custom method introduced to derived class.
 *
 * Overrides of this methods shall not throw.
 */
extern size_t transformation_sink_min(const transformation *this);

/*!
 * @fn transformation_source_min(const transformation *this)
 * @brief Returns minimum required amount of readable space in source buffer for transform method to be invoked
 * @memberof transformation
 * @param this Pointer to "this" transformation
 * @return Minimum required amount of readable space in source buffer for transform method to be invoked
 *
 * @ref controller guarantees that transformation_transform(const transformation *) will be invoked only when source
 * buffer has at least the required amount of readable space
 * (i.e. buffer_read_size(&this->base.source) >= transformation_source_min(this)).
 *
 * The returned value shall be higher than zero, and must NOT change as a result of any overridden method invocation
 * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
 * method. Value may change as a result of an invocation of any custom method introduced to derived class.
 *
 * Overrides of this methods shall not throw.
 */
extern size_t transformation_source_min(const transformation *this);

/*!
 * @fn transformation_set_source(buffer *source, transformation *this)
 * @brief Sets source buffer of a transformation object
 * @memberof transformation
 * @param source Pointer to the source buffer to use as this transformation's source
 * @param this Pointer to "this" transformation
 *
 * This method is automatically invoked by @ref controller when needed.
 */
extern void transformation_set_source(buffer *source, transformation *this);

/*!
 * @fn transformation_set_sink(buffer *sink, transformation *this)
 * @brief Sets sink buffer of a transformation object
 * @memberof transformation
 * @param source Pointer to the sink buffer to use as this transformation's sink
 * @param this Pointer to "this" transformation
 *
 * This method is automatically invoked by @ref controller when needed.
 */
extern void transformation_set_sink(buffer *sink, transformation *this);

#endif //CTRANSFORM_TRANSFORMATION_H
