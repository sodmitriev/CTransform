#include "source.h"

void source_destructor(source *this)
{
    this->call_tab->destructor(this);
}

size_t source_sink_min(const source *this)
{
    size_t ret = this->call_tab->sink_min(this);
    assert(ret > 0);
    return ret;
}

void source_send(source *this)
{
    assert(buffer_write_size(this->sink) >= this->call_tab->sink_min(this));
    assert(!this->call_tab->end(this));
    this->call_tab->send(this);
}

bool source_end(const source *this)
{
    return this->call_tab->end(this);
}

void source_set_sink(buffer *sink, source *this)
{
    assert(sink);
    this->sink = sink;
}
