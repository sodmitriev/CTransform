#include "transformation_b64_decode.h"
#include <string.h>
#include <assert.h>
#include <openssl/evp.h>
#include <CEasyException/exception.h>
#include <errno.h>

transformation_call_tab transformation_call_tab_b64_decode =
        {
                .destructor = (void (*)(transformation *)) transformation_b64_decode_destructor,
                .transform = (void (*)(transformation *)) transformation_b64_decode_transform,
                .finalize = (void (*)(transformation *)) transformation_b64_decode_finalize,
                .sink_min = (size_t (*)(const transformation *)) transformation_b64_decode_sink_min,
                .source_min = (size_t (*)(const transformation *)) transformation_b64_decode_source_min
        };

//Fix padding because OpenSSL doesn't give a damn
static size_t get_padding(const char* ptr, size_t size)
{
    //trailing '=' -> 1 byte of padding
    //trailing '==' -> 2 bytes of padding
    //trailing '===' -> wrong encoding (openssl should fail)
    //trailing '====' -> 3 bytes of padding
    size_t padding = 0;
    for(const char* end = ptr + size - 1; end >= ptr && *end != '+' && *end != '/' && (*end < 'a' || *end > 'z') && (*end < 'A' || *end > 'Z'); --end)
    {
        if(*end == '=')
        {
            ++padding;
        }
    }
    //For each 4 '=' there is 3 bytes missing
    return padding - (padding / 4);
}

void transformation_b64_decode_destructor(transformation_b64_decode* this)
{
}

void transformation_b64_decode_transform(transformation_b64_decode* this)
{
    assert(buffer_read_size(this->base.source) >= transformation_b64_decode_source_min(this));
    assert(buffer_write_size(this->base.sink) >= transformation_b64_decode_sink_min(this));
    int res = EVP_DecodeBlock(buffer_wpos(this->base.sink), buffer_rpos(this->base.source), 64);
    if(res < 0)
    {
        EXCEPTION_THROW(ENOANO, "%s",
                        "Failed to decode block from base64 (may be caused by spaces or new lines in input string)");
        return;
    }
    buffer_winc(res - get_padding(buffer_rpos(this->base.source), 64), this->base.sink);
    buffer_rinc(64, this->base.source);
}

void transformation_b64_decode_finalize(transformation_b64_decode* this)
{
    assert(buffer_read_size(this->base.source) < transformation_b64_decode_source_min(this));
    assert(buffer_write_size(this->base.sink) >= transformation_b64_decode_sink_min(this));
    size_t left = buffer_read_size(this->base.source);
    if(left > 0)
    {
        int res = EVP_DecodeBlock(buffer_wpos(this->base.sink), buffer_rpos(this->base.source), (int)left);
        if (res < 0)
        {
            EXCEPTION_THROW(ENOANO, "%s",
                            "Failed to decode block from base64 (may be caused by spaces or new lines in input string)");
            return;
        }
        buffer_winc(res - get_padding(buffer_rpos(this->base.source), left), this->base.sink);
        buffer_rinc(left, this->base.source);
    }
}

size_t transformation_b64_decode_sink_min(const transformation_b64_decode* this)
{
    return 48;
}

size_t transformation_b64_decode_source_min(const transformation_b64_decode* this)
{
    return 64;
}

void transformation_b64_decode_constructor(transformation_b64_decode* this)
{
    this->base.call_tab = &transformation_call_tab_b64_decode;
}