#include "transformation_remove.h"
#include <assert.h>
#include <string.h>
#include <limits.h>

transformation_call_tab transformation_call_tab_remove =
        {
                .destructor = (void (*)(transformation *))transformation_remove_destructor,
                .transform = (void (*)(transformation *))transformation_remove_transform,
                .finalize = (bool (*)(transformation *))transformation_remove_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_remove_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_remove_source_min
        };

void transformation_remove_transform(transformation_remove *this)
{
    assert(buffer_read_size(this->base.source) >= transformation_remove_source_min(this));
    assert(buffer_write_size(this->base.sink) >= transformation_remove_sink_min(this));
    char byte = *buffer_rpos(this->base.source);
    buffer_rinc(1, this->base.source);
    if(this->mask[(unsigned char)byte / 8] & (1u << (unsigned char)((unsigned char)byte % 8)))
    {
        return;
    }
    *buffer_wpos(this->base.sink) = byte;
    buffer_winc(1, this->base.sink);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool transformation_remove_finalize(transformation_remove *this)
{
    assert(buffer_read_size(this->base.source) < transformation_remove_source_min(this));
    assert(buffer_write_size(this->base.sink) >= transformation_remove_sink_min(this));
    return true;
}

size_t transformation_remove_sink_min(const transformation_remove *this)
{
    return 1;
}

size_t transformation_remove_source_min(const transformation_remove *this)
{
    return 1;
}

void transformation_remove_destructor(transformation_remove *this)
{
}

#pragma GCC diagnostic pop

void transformation_remove_constructor(const char *symbols, size_t size, transformation_remove *this)
{
    assert(sizeof(this->mask) * 8 > UCHAR_MAX);
    memset(&this->mask, 0, sizeof(this->mask));
    for(size_t i = 0; i < size; ++i)
    {
        this->mask[(unsigned char)symbols[i] / 8] |= (uint8_t)(1u << (unsigned char)((unsigned char)symbols[i] % 8));
    }
    this->base.call_tab = &transformation_call_tab_remove;
}
