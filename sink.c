#include "sink.h"

void sink_destructor(sink *this)
{
    this->call_tab->destructor(this);
}

size_t sink_source_min(const sink *this)
{
    return this->call_tab->source_min(this);
}

void sink_send(sink *this)
{
    this->call_tab->send(this);
}

bool sink_end(const sink *this)
{
    return this->call_tab->end(this);
}

void sink_set_source(buffer *source, sink *this)
{
    this->source = source;
}
