/*!
 * @file controller.h
 * @brief Controller class and methods declaration
 */

#ifndef CTRANSFORM_CONTROLLER_H
#define CTRANSFORM_CONTROLLER_H

#include "sink.h"
#include "source.h"
#include "transformation.h"
#include <CEasyException/exception.h>

/*!
 * @enum controller_stage
 * @brief Defines possible controller stages
 *
 * Refer to @ref Stages for description of controller stages
 */
typedef enum
{
    controller_stage_build, ///<build stage
    controller_stage_work,  ///<work stage
    controller_stage_final, ///<final stage
    controller_stage_done   ///<done stage
} controller_stage;

/*!
 * @class controller
 * @brief Objects of this class are used to maintain the transformation process
 *
 * Controller object is used to perform an iterative process of data transformation form @ref source to @ref sink.
 *
 * @section Structure
 *
 * Controller object includes a data @ref source (object that sends data to the controller system), a data @ref sink
 * (object that receives transformed data from controller system). In between source and sink zero, one ore more
 * sequent transformations may be inserted (of @ref transformation type). A buffer is added between each two elements
 * of the system (i.e. between source and first transformation, between each two adjacent transformations and between
 * last transformation and sink).
 *
 * A final process may look like this:
 *
 * [source] -> [buffer] -> [transformation 0] -> [buffer] -> ... -> [transformation N] -> [buffer] -> [sink]
 *
 * Or, in an extreme case, like this:
 *
 * [source] -> [buffer] -> [sink]
 *
 * Controller takes input data from source, applies a chain of transformations to this data, and sends the processed
 * data to sink. Transformations are applied in the same order they are added and may not be replaced after adding.
 * Source and sink may be replaced after setting in order to implement stream concatenation or batch processing.
 *
 * @section Stages
 *
 * Controller object has 4 sequent stages: @ref build, @ref work, @ref final and @ref done. Each stage has a
 * corresponding value in @ref controller_stage enum
 *
 * @subsection build
 *
 * Build stage lasts from invocation of controller_constructor(controller *) until first invocation of
 * controller_work(controller *)
 *
 * During this stage, controller must be supplied with a source, sink and a desired set of transformations
 *
 * The following methods can be called during this stage:
 * - controller_add_transformation(transformation *, controller *)
 * - controller_set_source(source *, controller *)
 * - controller_set_sink(sink *, controller *)
 * - controller_work(controller *) (advances controller to the @ref work stage)
 * - controller_finalize(controller *) (advances controller to the @ref final or @ref done stage)
 * - controller_get_stage(controller *)
 * - controller_destructor(controller *)
 *
 * @subsection work
 *
 * Work stage lasts from first invocation of controller_constructor(controller *) until first invocation of
 * controller_finalize(controller *)
 *
 * During this stage, data from source is partially transformed and sent to the sink. If controller is on this stage,
 * then there is no guarantee that all data is processed, as some of it may be left in the buffers.
 *
 * The following methods can be called during this stage:
 * - controller_set_source(source *, controller *)
 * - controller_set_sink(sink *, controller *)
 * - controller_work(controller *)
 * - controller_finalize(controller *) (advances controller to the @ref final or @ref done stage)
 * - controller_get_stage(controller *)
 * - controller_destructor(controller *)
 *
 * @subsection final
 *
 * Final stage lasts from first invocation of controller_finalize(controller *) until
 * controller_finalize(controller *) results in all data being processed and sent to the sink
 * (i.e. no data is left in the buffers)
 *
 * During this stage, buffers are flushed and transformations are finalized. If controller is on this stage,
 * then there is no guarantee that all data is processed, as some of it may be left in the buffers.
 *
 * The following methods can be called during this stage:
 * - controller_set_sink(sink *, controller *)
 * - controller_finalize(controller *) (advances controller to the @ref done stage when all data was processed)
 * - controller_get_stage(controller *)
 * - controller_destructor(controller *)
 *
 * @subsection done
 *
 * Done stage lasts from controller_finalize(controller *) results in all data being processed and sent to the sink
 * (i.e. no data is left in the buffers) and until the object is destructed
 *
 * If controller is on this stage, all of the data was successfully processed and object may be destroyed without any
 * data loss.
 *
 * The following methods can be called during this stage:
 * - controller_get_stage(controller *)
 * - controller_destructor(controller *)
 */
typedef struct
{
    //! @cond PRIVATE
    source *in;
    struct controller_buffer_node *first;
    struct controller_buffer_node *last;
    sink *out;
    struct controller_buffer_node *last_fin;
    controller_stage stage;
    //! @endcond
} controller;

/*!
 * @fn controller_constructor(controller *this)
 * @brief Initializes "this" controller
 * @memberof controller
 * @param this Pointer to a controller to construct
 * @throws ENOMEM Failed to allocate memory for buffer
 *
 * Sets both source and sink of "this" controller to NULL, initializes a transformation chain with no transformations and
 * creates a single buffer between source and sink.
 *
 * Shall only be invoked for a newly allocated or destructed controller.
 */
extern void controller_constructor(controller *this);

/*!
 * @fn controller_destructor(controller *this)
 * @brief Destroys "this" controller
 * @memberof controller
 * @param this Pointer to a controller to destruct
 *
 * Destructs all buffers created by "this" controller, frees all memory allocated by "this" controller.
 *
 * Does NOT destruct source, sink and added transformations. Destruction of those objects is user's responsibility.
 */
extern void controller_destructor(controller *this);

/*!
 * @fn controller_add_transformation(transformation *transform, controller *this)
 * @brief Adds a transformation to the end of controller's transformation chain
 * @memberof controller
 * @param transform Pointer to the added transformation
 * @param this Pointer to "this" controller object
 * @throws ENOMEM Failed to allocate memory for the buffer or transformation chain node
 *
 * Added transformation must satisfy all requirements of a @ref transformation object.
 *
 * Any modifications of an added transformation that will alter the produced data in any way are only allowed when
 * controller is on a @ref build stage. Modifications that do not alter the produced data in any way (including the ones
 * that affect thrown exceptions type and frequency) are allowed on any stage.
 *
 * Adding same transformation twice leads to an undefined behaviour.
 *
 * This method shall only be called while controller is on a @ref build stage.
 */
extern void controller_add_transformation(transformation *transform, controller *this);

/*!
 * @fn controller_set_source(source *in, controller *this)
 * @brief Sets data source of a controller
 * @memberof controller
 * @param in Pointer to a source to use
 * @param this Pointer to "this" controller object
 *
 * Added source must satisfy all requirements of a @ref source object.
 *
 * Added source may be replaced or modified in any desired way when controller is on @ref build or @ref work stages.
 * Replacement or alteration of the source in any way that affects the produced data (including it's quantity) is not
 * allowed on a @ref final stage.
 *
 * This method shall only be called while controller is on a @ref build or @ref work stage.
 */
extern void controller_set_source(source *in, controller *this);

/*!
 * @fn controller_set_sink(sink *in, controller *this)
 * @brief Sets data sink of a controller
 * @memberof controller
 * @param in Pointer to a sink to use
 * @param this Pointer to "this" controller object
 *
 * Added sink must satisfy all requirements of a @ref sink object.
 *
 * Added sink may be replaced or modified in any desired way when controller is any of it's stages stages except
 * @ref done.
 */
extern void controller_set_sink(sink *out, controller *this);

/*!
 * @fn controller_work(controller *this)
 * @brief Perform a work cycle
 * @memberof controller
 * @param this Pointer to "this" controller object
 * @throws ENOMEM Buffer reallocation failed
 * @throws ... Any exception thrown by source_send(source *) method of a provided data source
 * @throws ... Any exception thrown by transformation_transform(transformation *) method of any applied transformation
 * @throws ... Any exception thrown by sink_send(sink *) method of a provided data sink
 *
 * This method retrieves data from the source, transforms it with provided transformations and sends to the sink. Exact
 * order of those operations is unspecified (i.e. it may process all data in a single chunk, or may split data across
 * different chunks of a varying size for each transformation step).
 *
 * The process will continue until at least one of the following conditions is fulfilled:
 * - source reaches it's end (source_end(source *) returns true)
 * - sink is full (sink_end(sink *) returns true)
 * - source, sink or any transformation throws an exception (in this case, basic exception guarantee is provided)
 *
 * If source or sink reaches it's end as a result of this operation, user may modify or replace them in any required
 * way (although this condition is not a requirement, and modification of non finished sink and source is also allowed).
 *
 * If both source and sink are not limited, calling this method results in an infinite loop
 * (unless exception is thrown).
 *
 * It is possible that nothing will be written into sink after this method is invoked. This may happens when source had
 * no data or all data was spread across controller's buffers. It is not an error situation, if this behaviour is
 * undesired, use controller_finalize(controller *) method instead.
 *
 * This method shall only be called while controller is on either @ref build or @ref work stage.
 *
 * If controller is on a
 * @ref build stage, call to this method advances it into @ref work stage. In that case, sink, source and desired
 * transformations must be set beforehand with controller_set_source(source *, controller *),
 * controller_set_sink(sink *, controller *) and controller_add_transformation(transformation *, controller *)
 * methods respectively.
 */
extern void controller_work(controller *this);

/*!
 * @fn controller_finalize(controller *this)
 * @brief Perform a work cycle and flush all buffers and transformations
 * @memberof controller
 * @param this Pointer to "this" controller object
 * @throws ENOMEM Buffer reallocation failed
 * @throws ... Any exception thrown by source_send(source *) method of a provided data source
 * @throws ... Any exception thrown by transformation_transform(transformation *) method of any applied transformation
 * @throws ... Any exception thrown by transformation_finalize(transformation *) method of any applied transformation
 * @throws ... Any exception thrown by sink_send(sink *) method of a provided data sink
 *
 * This method retrieves data from the source, transforms it with provided transformations and sends to the sink. Exact
 * order of those operations is unspecified (i.e. it may process all data in a single chunk, or may split data across
 * different chunks of a varying size for each transformation step).
 *
 * When source reaches it's ending, controller flushes all buffers and finalizes all transformations (in order they are
 * located from source to sink).
 *
 * The process will continue until at least one of the following conditions is fulfilled:
 * - source is empty, all transformations are successfully finalized and sink received all the output data
 * - sink is full (sink_end(sink *) returns true)
 * - source, sink or any transformation throws an exception (in this case, basic exception guarantee is provided)
 *
 * If all buffers were successfully flushed, all transformations were successfully finalized and sink received all
 * resulting data, "this" controller moves to the @ref done stage. If not,user may modify or replace sink in
 * any required way, and then invoke this method again. In subsequent invocations, controller will start flushing from
 * the point it left it the last time.
 *
 * After this method has returned, user should check controller stage (via controller_get_stage(controller *)) to
 * check if process has finished, or if this method should be invoked again.
 *
 * If both source and sink are not limited, calling this method results in an infinite loop
 * (unless exception is thrown).
 *
 * This method may be called on any controller's stage, except for @ref done stage.
 *
 * If controller is on a @ref build
 * stage, call to this method advances it into @ref final stage, skipping the @ref work stage. In that case, sink, source and
 * desired transformations must be set beforehand with controller_set_source(source *, controller *),
 * controller_set_sink(sink *, controller *) and controller_add_transformation(transformation *, controller *) methods
 * respectively.
 *
 * If controller is on @ref work stage, call to this method advances it into @ref final stage.
 *
 * If all data was successfully transformed and sent to sink, this method will advance controller to the @ref done stage
 * regardless of it's current stage.
 */
extern void controller_finalize(controller *this);

/*!
 * @fn controller_get_stage(controller *this)
 * @brief Get current controller stage
 * @memberof controller
 * @param this Pointer to "this" controller object
 * @return Current stage of a controller
 *
 * For stage description refer to @ref Stages section of controller class
 */
extern controller_stage controller_get_stage(controller *this);

#endif //CTRANSFORM_CONTROLLER_H
