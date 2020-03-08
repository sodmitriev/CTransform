#include "controller.h"
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

#define CONTROLLER_BUF_MIN_SIZE 4096

#define RETHROW_EXCEPTION() \
    if(EXCEPTION_IS_THROWN) \
    {                       \
        return;             \
    }((void)(0))            \

// List node that contains a buffer
typedef struct controller_buffer_node
{
    buffer buffer;
    struct controller_transformation_node *next;
} controller_buffer_node;

//List node that contains a transformations
typedef struct controller_transformation_node
{
    transformation *transform;
    struct controller_buffer_node *next;
} controller_transformation_node;

void controller_constructor(controller *this)
{
    //Init everything to NULL
    this->in = NULL;
    this->out = NULL;
    this->first = NULL;
    this->last = NULL;
    this->last_fin = NULL;
    //Set stage to "build"
    this->stage = controller_stage_build;
    //Allocate a node for a first (located between source and sink)
    controller_buffer_node *node = malloc(sizeof(controller_buffer_node));
    if(!node)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    node->next = NULL;
    //Allocate buffer of a created node
    buffer_constructor(CONTROLLER_BUF_MIN_SIZE, &node->buffer);
    if(EXCEPTION_IS_THROWN)
    {
        free(node);
        return;
    }
    //Link created node to the list
    this->first = node;
    this->last = node;
}

void controller_destructor(controller *this)
{
    assert(this->first);
    //Destroy the list
    controller_transformation_node *node = this->first->next;
    while(node != NULL)
    {
        assert(node->next); //There always should be a buffer after a transformation
        //Destroy buffer
        controller_transformation_node *next = node->next->next;
        //Free buffer node
        buffer_destructor(&node->next->buffer);
        node->next->next = NULL;
        free(node->next);
        //Free transformation node
        node->next = NULL;
        free(node);
        //Advance to the next node
        node = next;
    }
    //Destroy the first buffer
    buffer_destructor(&this->first->buffer);
    //Free the first node
    this->first->next = NULL;
    free(this->first);
    //Set memory to NULL
    this->first = NULL;
    this->last = NULL;
    this->last_fin = NULL;
    //Set stage to done
    this->stage = controller_stage_done;
}

void controller_add_transformation(transformation *transform, controller *this)
{
    assert(this->first);
    assert(this->last);
    assert(this->stage == controller_stage_build); //Only allowed on build stage
    //Allocate a transformation node
    controller_transformation_node *trans_node = malloc(sizeof(controller_transformation_node));
    //Allocate a buffer node
    controller_buffer_node *buf_node = malloc(sizeof(controller_buffer_node));
    if(!trans_node || !buf_node)
    {
        free(trans_node);
        free(buf_node);
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    //Construct a buffer in the allocated buffer node
    buffer_constructor(CONTROLLER_BUF_MIN_SIZE, &buf_node->buffer);
    if(EXCEPTION_IS_THROWN)
    {
        free(trans_node);
        free(buf_node);
        return;
    }
    //Set transformation in the allocated transformation node
    trans_node->transform = transform;
    //Link allocated nodes to the list
    trans_node->next = buf_node;
    buf_node->next = NULL;
    this->last->next = trans_node;
    this->last = buf_node;
}

void controller_set_source(source *in, controller *this)
{
    //Ony allowed on build or work stages
    assert(this->stage == controller_stage_build || this->stage == controller_stage_work);
    this->in = in;
}

void controller_set_sink(sink *out, controller *this)
{
    //Not allowed when done (because why would you need this?)
    assert(this->stage != controller_stage_done);
    this->out = out;
}

//Sets buffer size to the size required by the adjacent elements
static void adjust_buffer(size_t size, buffer *buf)
{
    //Check for possible overflow
    //If you have more than 9 exabyte of RAM you probably use cluster and won't use this single thread software
    assert(ULLONG_MAX / 2 > size);
    //Set desired size to 1.5 of original size to reduce amount of iterations
    size = (size / 2) * 3;
    //If resulting size is less than minimum, set it to minimum
    if(size < CONTROLLER_BUF_MIN_SIZE)
    {
        size = CONTROLLER_BUF_MIN_SIZE;
    }
    size_t cur_size = buffer_size(buf);
    //If current size is between size and (2 * size) then leave it that way to avoid extra memory reallocation
    //(((1.5 * size)/3) * 2 == size, ((1.5 * size)/3) * 4 == 2 * size)
    if(cur_size < (size / 3) * 2 || cur_size > (size / 3) * 4)
    {
        //If buffer has more data than size, set size to the amount of data, because we don't want to lose that data
        if(size < buffer_occupied(buf))
        {
            size = buffer_occupied(buf);
        }
        //Perform resize operation
        buffer_resize(size, buf);
        RETHROW_EXCEPTION();
    }
}

//Sets all buffer sizes to the size required by the adjacent elements
static void adjust_buffers(controller *this)
{
    //Remember sink size required by source
    size_t prev_min = source_sink_min(this->in);
    //Iterate over transformation chain
    for(controller_buffer_node *node = this->first; node != this->last; node = node->next->next)
    {
        //Require a size of at least (sink requirement for previous element + source requirement of the next element)
        //This is used to avoid deadlocks
        adjust_buffer(prev_min + transformation_source_min(node->next->transform), &node->buffer);
        RETHROW_EXCEPTION();
        //Set previous node sink size requirement to this node's requirement
        prev_min = transformation_sink_min(node->next->transform);
    }
    //Adjust the last buffer
    adjust_buffer(prev_min + sink_source_min(this->out), &this->last->buffer);
}

//Link all elements with adjacent buffers
static void link_sink_source(controller *this)
{
    //Set sink for source
    source_set_sink(&this->first->buffer, this->in);
    //Set source and sink for each transformation
    for(controller_buffer_node *node = this->first; node != this->last; node = node->next->next)
    {
        transformation_set_source(&node->buffer, node->next->transform);
        transformation_set_sink(&node->next->next->buffer, node->next->transform);
    }
    //Set source for sink
    sink_set_source(&this->last->buffer, this->out);
}

//Free buffers of data that was already read
static void compact_buffers(controller_buffer_node *from, controller_buffer_node *to)
{
    //Compact each buffer except last one
    for(controller_buffer_node *node = from; node != to; node = node->next->next)
    {
        buffer_compact(&node->buffer);
    }
    //Compact last buffer
    buffer_compact(&to->buffer);
}

//Send as much data from source to first buffer as possible
static void controller_work_source(controller *this)
{
    //Send while all requirements are fulfilled
    while(!source_end(this->in) && buffer_write_size(&this->first->buffer) >= source_sink_min(this->in))
    {
        source_send(this->in);
        RETHROW_EXCEPTION();
    }
}

//Transform as much data as possible
static void controller_work_transformations(controller_buffer_node *from, controller_buffer_node *to)
{
    //For each transformation
    for(controller_buffer_node *node = from; node != to; node = node->next->next)
    {
        //Transform while all requirements are fulfilled
        while(buffer_read_size(&node->buffer) >= transformation_source_min(node->next->transform) &&
              buffer_write_size(&node->next->next->buffer) >= transformation_sink_min(node->next->transform))
        {
            transformation_transform(node->next->transform);
            RETHROW_EXCEPTION();
        }
    }
}

//Send as much data from last buffer to sink as possible
static void controller_work_sink(controller *this)
{
    //Send while all requirements are fulfilled
    while(buffer_read_size(&this->last->buffer) >= sink_source_min(this->out) && !sink_end(this->out))
    {
        sink_send(this->out);
        RETHROW_EXCEPTION();
    }
}

//Preforms data processing until source can send and sink can receive
static void controller_work_cycle(controller *this)
{
    //Adjust buffers because requirements might have changed
    adjust_buffers(this);
    RETHROW_EXCEPTION();
    //Link because source/sink might have changed, or this may be the first call
    link_sink_source(this);
    //While source can send and sink can receive
    while(!source_end(this->in) && !sink_end(this->out))
    {
        //Free buffers of data that was already processed
        compact_buffers(this->first, this->last);
        //Get maximum amount of data from source
        controller_work_source(this);
        RETHROW_EXCEPTION();
        //Transform maximum amount of data
        controller_work_transformations(this->first, this->last);
        RETHROW_EXCEPTION();
        //Send maximum amount of data to sink
        controller_work_sink(this);
        RETHROW_EXCEPTION();
    }
}

void controller_work(controller *this)
{
    //Only allowed on build and work stages
    assert(this->stage == controller_stage_build || this->stage == controller_stage_work);
    //Disable further transformation modifications by moving to build stage
    this->stage = controller_stage_work;
    //Perform the work cycle
    controller_work_cycle(this);
}

void controller_finalize(controller *this)
{
    //Not allowed when done (because calling this when done is useless and should be avoided)
    assert(this->stage != controller_stage_done);
    //Set stage to final
    this->stage = controller_stage_final;
    if(!this->last_fin)
    {
        //If finalization is not started, perform a regular work cycle to empty the source
        controller_work_cycle(this);
        RETHROW_EXCEPTION();
        //Exit sink gets full instead
        if(sink_end(this->out))
        {
            return;
        }
        assert(source_end(this->in));
        //Set next node to finalize to first node
        this->last_fin = this->first;
    }
    else
    {
        //If finalization already started, just fix the transformation chain
        adjust_buffers(this);
        RETHROW_EXCEPTION();
        link_sink_source(this);
    }
    //For each buffer
    while(this->last_fin != this->last)
    {
        //Work until this buffer flushes maximum data
        while(buffer_read_size(&this->last_fin->buffer) >= transformation_source_min(this->last_fin->next->transform))
        {
            //Remove data that is already processed
            compact_buffers(this->last_fin, this->last);
            //Transform as much as possible starting from node that is being finalized
            controller_work_transformations(this->last_fin, this->last);
            RETHROW_EXCEPTION();
            //Send maximum possible amount of data to sink
            controller_work_sink(this);
            RETHROW_EXCEPTION();
            //Finish if we can't write any more data
            if(sink_end(this->out))
            {
                return;
            }
        }
        //Free enough space in this current transformation's sink
        while(buffer_write_size(&this->last_fin->next->next->buffer) <
              transformation_sink_min(this->last_fin->next->transform))
        {
            //Remove data that is already processed
            compact_buffers(this->last_fin, this->last);
            //Transform as much as possible starting from node next to the node that is being finalized
            controller_work_transformations(this->last_fin->next->next, this->last);
            RETHROW_EXCEPTION();
            //Send maximum possible amount of data to sink
            controller_work_sink(this);
            RETHROW_EXCEPTION();
            //Finish if we can't write any more data
            if(sink_end(this->out))
            {
                return;
            }
        }
        //Try finalizing current transformation
        bool ret;
        do
        {
            ret = transformation_finalize(this->last_fin->next->transform);
            RETHROW_EXCEPTION();
        } while(!ret
                && buffer_write_size(&this->last_fin->next->next->buffer) >=
                   transformation_sink_min(this->last_fin->next->transform));
        //Advance if finished, else repeat all again for the same buffer
        if(ret)
        {
            this->last_fin = this->last_fin->next->next;
        }
    }
    //Flush whatever is left from last transformation finalization
    controller_work_sink(this);
    RETHROW_EXCEPTION();
    //If all flushed successfully, move to the done stage
    if(buffer_read_size(&this->last->buffer) < sink_source_min(this->out))
    {
        this->stage = controller_stage_done;
    }
}

controller_stage controller_get_stage(controller *this)
{
    return this->stage;
}
