#include "sink_gets.h"
#include <string.h>
#include <assert.h>

sink_call_tab sink_call_tab_gets =
        {
                .destructor  = (void (*)(sink *))sink_gets_destructor,
                .source_min  = (size_t (*)(const sink *))sink_gets_source_min,
                .send        = (void (*)(sink *))sink_gets_send,
                .end         = (bool (*)(const sink *))sink_gets_end
        };

void sink_gets_constructor(sink_gets *this)
{
    this->base.call_tab = &sink_call_tab_gets;
    this->base.source = NULL;
    this->cnt = 0;
    this->finished = true;
}

void sink_gets_destructor(sink_gets *this)
{
    this->base.source = NULL;
    this->s = NULL;
    this->size = 0;
    this->cnt = 0;
    this->finished = true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
size_t sink_gets_source_min(const sink_gets *this)
{
    return 1;
}
#pragma GCC diagnostic pop

void sink_gets_send(sink_gets *this)
{
    size_t src_cnt = buffer_read_size(this->base.source);
    size_t need_cnt = this->size - 1 - this->cnt;
    size_t count = need_cnt < src_cnt ? need_cnt : src_cnt;
    char* pos = memchr(buffer_rpos(this->base.source), '\n', count);
    if(pos)
    {
        count = pos - buffer_rpos(this->base.source) + 1;
    }
    memcpy(this->s + this->cnt, buffer_rpos(this->base.source), count);
    buffer_rinc(count, this->base.source);
    this->cnt += count;
    this->s[this->cnt] = '\0';
    assert(this->cnt < this->size);
    if(pos != NULL || this->cnt == this->size - 1)
    {
        this->finished = true;
    }
}

bool sink_gets_end(const sink_gets *this)
{
    return this->finished;
}

void sink_gets_set(char *s, int size, sink_gets *this)
{
    this->s = s;
    assert(size >0);
    this->size = size;
    this->cnt = 0;
    this->finished = false;
}

char * sink_gets_get_result(const sink_gets *this)
{
    if(this->cnt == 0)
    {
        return NULL;
    }
    return this->s;
}
