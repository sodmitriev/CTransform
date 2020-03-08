#include "sink.h"

void sink_destructor(sink *this)
{
    this->call_tab->destructor(this);
}

size_t sink_source_min(const sink *this)
{
    size_t ret = this->call_tab->source_min(this);
    assert(ret > 0);
    return ret;
}

void sink_send(sink *this)
{
    assert(buffer_read_size(this->source) >= this->call_tab->source_min(this));
    assert(!this->call_tab->end(this));
    this->call_tab->send(this);
}

bool sink_end(const sink *this)
{
    return this->call_tab->end(this);
}

void sink_set_source(buffer *source, sink *this)
{
    assert(source);
    this->source = source;
}
