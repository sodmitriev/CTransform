#include <buffer/buffer.h>
#include <CEasyException/exception.h>
#include "transformation_encrypt.h"
#include "transformation_decrypt.h"
#include <stdlib.h>
#include <assert.h>

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    fflush(stderr);                         \
    abort();                                \
}((void)(0))

int main()
{
    const char msg[] = "0000000000000000"
                       "1111111111111111"
                       "2222222222222222"
                       "3333333333333333"
                       "4444444444444444"
                       "5555555555555555"
                       "6666666666666666"
                       "7777777777777777"
                       "8888888888888888"
                       "9999999999999999"
                       "aaaaaaaaaaaaaaaa"
                       "bbbbbbbbbbbbbbbb"
                       "cccccccccccccccc"
                       "dddddddddddddddd"
                       "eeeeeeeeeeeeeeee"
                       "fffffff";
    buffer in;
    buffer mid;
    buffer out;
    transformation_encrypt encrypt;
    transformation_decrypt decrypt;

    EXCEPTION_CLEAR();

    buffer_constructor(sizeof(msg), &in);
    HANDLE_EXCEPTION();
    buffer_constructor(64, &mid);
    HANDLE_EXCEPTION();
    buffer_constructor(sizeof(msg) + 128, &out);
    HANDLE_EXCEPTION();

    memcpy(in.buf, msg, sizeof(msg));
    in.wpos = sizeof(msg);


    transformation_encrypt_constructor("aes-256-cbc", "sha1", "mykey", &encrypt);
    HANDLE_EXCEPTION();
    transformation_decrypt_constructor("aes-256-cbc", "sha1", "mykey", &decrypt);
    HANDLE_EXCEPTION();

    assert(buffer_size(&in) >= transformation_source_min(&encrypt));
    assert(buffer_size(&mid) >= transformation_sink_min(&encrypt));
    assert(buffer_size(&mid) >= transformation_source_min(&decrypt));
    assert(buffer_size(&out) >= transformation_sink_min(&decrypt));

    transformation_set_source(&in, &encrypt);
    transformation_set_sink(&mid, &encrypt);
    transformation_set_source(&mid, &decrypt);
    transformation_set_sink(&out, &decrypt);

    while((buffer_readable(&in) || buffer_readable(&mid)) && buffer_writable(&out))
    {
        if(!buffer_readable(&in))
        {
            buffer_reset(&in);
        }
        if(!buffer_readable(&mid))
        {
            buffer_reset(&mid);
        }
        if(buffer_write_size(&mid) >= transformation_sink_min(&encrypt))
        {
            transformation_transform(&encrypt);
            HANDLE_EXCEPTION();
        }
        assert(buffer_write_size(&out) >= transformation_sink_min(&decrypt));
        transformation_transform(&decrypt);
        HANDLE_EXCEPTION();
    }
    assert(buffer_write_size(&mid) >= transformation_sink_min(&encrypt));
    transformation_finalize(&encrypt);
    HANDLE_EXCEPTION();
    while(buffer_readable(&mid) && buffer_writable(&out))
    {
        if(!buffer_readable(&mid))
        {
            buffer_reset(&mid);
        }
        assert(buffer_write_size(&out) >= transformation_sink_min(&decrypt));
        transformation_transform(&decrypt);
        HANDLE_EXCEPTION();
    }
    transformation_finalize(&decrypt);
    HANDLE_EXCEPTION();
    assert(strcmp(msg, out.buf) == 0);
    assert(out.wpos == sizeof(msg));
    transformation_destructor(&encrypt);
    transformation_destructor(&decrypt);
    buffer_destructor(&in);
    buffer_destructor(&mid);
    buffer_destructor(&out);
    return 0;
}
