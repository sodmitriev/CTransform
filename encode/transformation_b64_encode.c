#include "transformation_b64_encode.h"
#include <assert.h>
#include <openssl/evp.h>

transformation_call_tab transformation_call_tab_b64_encode =
        {
                .destructor = (void (*)(transformation *))transformation_b64_encode_destructor,
                .transform = (void (*)(transformation *))transformation_b64_encode_transform,
                .finalize = (void (*)(transformation *))transformation_b64_encode_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_b64_encode_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_b64_encode_source_min
        };

void transformation_b64_encode_transform(transformation_b64_encode *this)
{
    assert(buffer_read_size(this->base.source) >= transformation_b64_encode_source_min(this));
    assert(buffer_write_size(this->base.sink) >= transformation_b64_encode_sink_min(this));
    int written = EVP_EncodeBlock((unsigned char *)buffer_wpos(this->base.sink),
                                  (const unsigned char *)buffer_rpos(this->base.source), 48);
    assert(written == 64);
    buffer_rinc(48, this->base.source);
    buffer_winc(64, this->base.sink);
}

void transformation_b64_encode_finalize(transformation_b64_encode *this)
{
    assert(buffer_read_size(this->base.source) < transformation_b64_encode_source_min(this));
    assert(buffer_write_size(this->base.sink) >= transformation_b64_encode_sink_min(this));
    if(buffer_readable(this->base.source))
    {
        int written = EVP_EncodeBlock((unsigned char *)buffer_wpos(this->base.sink),
                                      (const unsigned char *)buffer_rpos(this->base.source),
                                      (int)buffer_read_size(this->base.source));
        assert(written >= 0); //Openssl should guarantee this
        buffer_rinc(buffer_read_size(this->base.source), this->base.source);
        buffer_winc((size_t)written, this->base.sink);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

size_t transformation_b64_encode_sink_min(const transformation_b64_encode *this)
{
    //64 bytes of block + '\0' ('\0' will be omitted)
    return 65;
}

size_t transformation_b64_encode_source_min(const transformation_b64_encode *this)
{
    return 48;
}

void transformation_b64_encode_destructor(transformation_b64_encode *this)
{
}

#pragma GCC diagnostic pop

void transformation_b64_encode_constructor(transformation_b64_encode *this)
{
    this->base.call_tab = &transformation_call_tab_b64_encode;
}
