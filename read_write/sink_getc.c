#include "sink_getc.h"
#include <assert.h>
#include <stdio.h>

sink_call_tab sink_call_tab_getc =
        {
                .destructor  = (void (*)(sink *))sink_getc_destructor,
                .source_min  = (size_t (*)(const sink *))sink_getc_source_min,
                .send        = (void (*)(sink *))sink_getc_send,
                .end         = (bool (*)(const sink *))sink_getc_end
        };

void sink_getc_constructor(sink_getc *this)
{
    this->base.call_tab = &sink_call_tab_getc;
    this->base.source = NULL;
    this->received = true;
}

void sink_getc_destructor(sink_getc *this)
{
    this->base.source = NULL;
    this->received = true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t sink_getc_source_min(const sink_getc *this)
{
    return 1;
}

#pragma GCC diagnostic pop

void sink_getc_send(sink_getc *this)
{
    this->chr = *buffer_rpos(this->base.source);
    buffer_rinc(1, this->base.source);
    this->received = true;
}

bool sink_getc_end(const sink_getc *this)
{
    return this->received;
}

void sink_getc_set(sink_getc *this)
{
    this->received = false;
}

int sink_getc_get_result(const sink_getc *this)
{
    if(!this->received)
    {
        return EOF;
    }
    return (int)this->chr;
}
