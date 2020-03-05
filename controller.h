#ifndef CTRANSFORM_CONTROLLER_H
#define CTRANSFORM_CONTROLLER_H

#include "sink.h"
#include "source.h"
#include "transformation.h"
#include <CEasyException/exception.h>

typedef struct
{
    source *in;
    struct controller_buffer_node *first;
    struct controller_buffer_node *last;
    sink *out;
    struct controller_buffer_node *last_fin;
} controller;

extern void controller_constructor(controller *this);

extern void controller_destructor(controller *this);

extern void controller_add_transformation(transformation *transform, controller *this);

extern void controller_set_source(source *in, controller *this);

extern void controller_set_sink(sink *out, controller *this);

extern void controller_work(controller *this);

extern void controller_finalize(controller *this);

#endif //CTRANSFORM_CONTROLLER_H
