#include "source.h"

void source_destructor(source *this)
{
    this->call_tab->destructor(this);
}

size_t source_sink_min(const source *this)
{
    return this->call_tab->sink_min(this);
}

void source_send(source *this)
{
    this->call_tab->send(this);
}

bool source_end(const source *this)
{
    return this->call_tab->end(this);
}

void source_set_sink(buffer *sink, source *this)
{
    this->sink = sink;
}
