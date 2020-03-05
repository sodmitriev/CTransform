#include "transformation.h"
#include <assert.h>

void transformation_destructor(transformation *this)
{
    this->call_tab->destructor(this);
    this->source = NULL;
    this->sink = NULL;
}

void transformation_transform(transformation *this)
{
    assert(this->source);
    assert(this->sink);
    this->call_tab->transform(this);
}

void transformation_finalize(transformation *this)
{
    this->call_tab->finalize(this);
}

size_t transformation_sink_min(const transformation *this)
{
    return this->call_tab->sink_min(this);
}

size_t transformation_source_min(const transformation *this)
{
    return this->call_tab->source_min(this);
}

void transformation_set_source(buffer *source, transformation *this)
{
    this->source = source;
}

void transformation_set_sink(buffer *sink, transformation *this)
{
    this->sink = sink;
}
