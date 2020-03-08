#include "source_write.h"
#include <string.h>

source_call_tab source_call_tab_write =
        {
                .destructor  = (void (*)(source *))source_write_destructor,
                .send        = (void (*)(source *))source_write_send,
                .sink_min    = (size_t (*)(const source *))source_write_sink_min,
                .end         = (bool (*)(const source *))source_write_end
        };

void source_write_constructor(source_write *this)
{
    this->base.call_tab = &source_call_tab_write;
    this->base.sink = NULL;
}

void source_write_destructor(source_write *this)
{
    this->base.sink = NULL;
    this->ptr = NULL;
    this->size = 0;
    this->nmemb = 0;
    this->cnt = 0;
}

void source_write_send(source_write *this)
{
    size_t sink_cnt = buffer_write_size(this->base.sink) / this->size;
    size_t have_cnt = this->nmemb - this->cnt;
    size_t count = have_cnt < sink_cnt ? have_cnt : sink_cnt;
    memcpy(buffer_wpos(this->base.sink), (const char *)this->ptr + this->cnt * this->size, count * this->size);
    buffer_winc(count * this->size, this->base.sink);
    this->cnt += count;
}

size_t source_write_sink_min(const source_write *this)
{
    return this->size;
}

bool source_write_end(const source_write *this)
{
    return this->cnt >= this->nmemb;
}

void source_write_set(const void *ptr, size_t size, size_t nmemb, source_write *this)
{
    this->ptr = ptr;
    this->size = size;
    this->nmemb = nmemb;
    this->cnt = 0;
}

size_t source_write_get_result(const source_write *this)
{
    return this->cnt;
}
