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

typedef struct controller_buffer_node
{
    buffer buffer;
    struct controller_transformation_node *next;
} controller_buffer_node;

typedef struct controller_transformation_node
{
    transformation *transform;
    struct controller_buffer_node *next;
} controller_transformation_node;

void controller_constructor(controller *this)
{
    this->in = NULL;
    this->out = NULL;
    this->first = NULL;
    this->last = NULL;
    this->last_fin = NULL;
    controller_buffer_node *node = malloc(sizeof(controller_buffer_node));
    if(!node)
    {
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    node->next = NULL;
    //Possible ENOMEM exception
    buffer_constructor(CONTROLLER_BUF_MIN_SIZE, &node->buffer);
    if(EXCEPTION_IS_THROWN)
    {
        free(node);
        return;
    }
    this->first = node;
    this->last = node;
    this->stage = controller_stage_build;
}

void controller_destructor(controller *this)
{
    assert(this->first);
    controller_transformation_node *node = this->first->next;
    while(node != NULL)
    {
        assert(node->next);
        controller_transformation_node *next = node->next->next;
        buffer_destructor(&node->next->buffer);
        node->next->next = NULL;
        free(node->next);
        node->next = NULL;
        free(node);
        node = next;
    }
    buffer_destructor(&this->first->buffer);
    this->first->next = NULL;
    free(this->first);
    this->first = NULL;
    this->last = NULL;
    this->last_fin = NULL;
    this->stage = controller_stage_done;
}

void controller_add_transformation(transformation *transform, controller *this)
{
    assert(this->first);
    assert(this->last);
    assert(this->stage == controller_stage_build);
    controller_transformation_node *trans_node = malloc(sizeof(controller_transformation_node));
    controller_buffer_node *buf_node = malloc(sizeof(controller_buffer_node));
    if(!trans_node || !buf_node)
    {
        free(trans_node);
        free(buf_node);
        EXCEPTION_THROW_NOMSG(ENOMEM);
        return;
    }
    buffer_constructor(CONTROLLER_BUF_MIN_SIZE, &buf_node->buffer);
    if(EXCEPTION_IS_THROWN)
    {
        free(trans_node);
        free(buf_node);
        return;
    }
    trans_node->transform = transform;
    trans_node->next = buf_node;
    buf_node->next = NULL;
    this->last->next = trans_node;
    this->last = buf_node;
}

void controller_set_source(source *in, controller *this)
{
    assert(this->stage == controller_stage_build || this->stage == controller_stage_work);
    this->in = in;
}

void controller_set_sink(sink *out, controller *this)
{
    assert(this->stage != controller_stage_done);
    this->out = out;
}

static void adjust_buffer(size_t size, buffer *buf)
{
    //If you have more than 9 exabyte of RAM you probably use cluster and won't use this single thread software
    assert(ULLONG_MAX / 2 > size);
    size = (size / 2) * 3;
    if(size < CONTROLLER_BUF_MIN_SIZE)
    {
        size = CONTROLLER_BUF_MIN_SIZE;
    }
    size_t cur_size = buffer_size(buf);
    if(cur_size < (size / 3) * 2 || cur_size > (size / 3) * 4)
    {
        if(size < buffer_occupied(buf))
        {
            size = buffer_occupied(buf);
        }
        buffer_resize(size, buf);
        RETHROW_EXCEPTION();
    }
}

static void adjust_buffers(controller *this)
{
    size_t prev_min = source_sink_min(this->in);
    for(controller_buffer_node *node = this->first; node != this->last; node = node->next->next)
    {
        adjust_buffer(prev_min + transformation_source_min(node->next->transform), &node->buffer);
        RETHROW_EXCEPTION();
        prev_min = transformation_sink_min(node->next->transform);
    }
    adjust_buffer(prev_min + sink_source_min(this->out), &this->last->buffer);
}

static void link_sink_source(controller *this)
{
    source_set_sink(&this->first->buffer, this->in);
    for(controller_buffer_node *node = this->first; node != this->last; node = node->next->next)
    {
        transformation_set_source(&node->buffer, node->next->transform);
        transformation_set_sink(&node->next->next->buffer, node->next->transform);
    }
    sink_set_source(&this->last->buffer, this->out);
}

static void compact_buffers(controller_buffer_node *from, controller_buffer_node *to)
{
    for(controller_buffer_node *node = from; node != to; node = node->next->next)
    {
        buffer_compact(&node->buffer);
    }
    buffer_compact(&to->buffer);
}

static void controller_work_source(controller *this)
{
    while(!source_end(this->in) && buffer_write_size(&this->first->buffer) >= source_sink_min(this->in))
    {
        source_send(this->in);
        RETHROW_EXCEPTION();
    }
}

static void controller_work_transformations(controller_buffer_node *from, controller_buffer_node *to)
{
    for(controller_buffer_node *node = from; node != to; node = node->next->next)
    {
        while(buffer_read_size(&node->buffer) >= transformation_source_min(node->next->transform) &&
              buffer_write_size(&node->next->next->buffer) >= transformation_sink_min(node->next->transform))
        {
            transformation_transform(node->next->transform);
            RETHROW_EXCEPTION();
        }
    }
}

static void controller_work_sink(controller *this)
{
    while(buffer_read_size(&this->last->buffer) >= sink_source_min(this->out) && !sink_end(this->out))
    {
        sink_send(this->out);
        RETHROW_EXCEPTION();
    }
}

static void controller_work_cycle(controller *this)
{
    adjust_buffers(this);
    RETHROW_EXCEPTION();
    link_sink_source(this);
    while(!source_end(this->in) && !sink_end(this->out))
    {
        compact_buffers(this->first, this->last);
        controller_work_source(this);
        RETHROW_EXCEPTION();
        controller_work_transformations(this->first, this->last);
        RETHROW_EXCEPTION();
        controller_work_sink(this);
        RETHROW_EXCEPTION();
    }
}

void controller_work(controller *this)
{
    assert(this->stage == controller_stage_build || this->stage == controller_stage_work);
    this->stage = controller_stage_work;
    controller_work_cycle(this);
}

void controller_finalize(controller *this)
{
    assert(this->stage != controller_stage_done);
    this->stage = controller_stage_final;
    if(!this->last_fin)
    {
        controller_work_cycle(this);
        RETHROW_EXCEPTION();
        if(sink_end(this->out))
        {
            return;
        }
        assert(source_end(this->in));
        this->last_fin = this->first;
    }
    else
    {
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
            compact_buffers(this->last_fin, this->last);
            controller_work_transformations(this->last_fin, this->last);
            RETHROW_EXCEPTION();
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
            compact_buffers(this->last_fin, this->last);
            controller_work_transformations(this->last_fin->next->next, this->last);
            RETHROW_EXCEPTION();
            controller_work_sink(this);
            RETHROW_EXCEPTION();
            //Finish if we can't write any more data
            if(sink_end(this->out))
            {
                return;
            }
        }
        bool ret = transformation_finalize(this->last_fin->next->transform);
        RETHROW_EXCEPTION();
        //Advance if finished, else repeat all again for the same buffer
        if(ret)
        {
            this->last_fin = this->last_fin->next->next;
        }
    }
    //Flush whatever is left from last transformation finalization
    controller_work_sink(this);
    RETHROW_EXCEPTION();
    if(buffer_read_size(&this->last->buffer) < sink_source_min(this->out))
    {
        this->stage = controller_stage_done;
    }
}

controller_stage controller_get_stage(controller *this)
{
    return this->stage;
}
