#include <buffer/buffer.h>
#include <CEasyException/exception.h>
#include "transformation_b64_encode.h"
#include "transformation_b64_decode.h"
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

    EXCEPTION_CLEAR();

    //Base 64
    {
        buffer in;
        buffer mid;
        buffer out;
        transformation_b64_encode encode;
        transformation_b64_decode decode;

        buffer_constructor(sizeof(msg), &in);
        HANDLE_EXCEPTION();
        buffer_constructor(80, &mid);
        HANDLE_EXCEPTION();
        buffer_constructor(sizeof(msg) + sizeof(msg) / 2, &out);
        HANDLE_EXCEPTION();

        memcpy(in.buf, msg, sizeof(msg));
        in.wpos = sizeof(msg);

        transformation_b64_encode_constructor(&encode);
        HANDLE_EXCEPTION();
        transformation_b64_decode_constructor(&decode);
        HANDLE_EXCEPTION();

        assert(buffer_size(&in) >= transformation_source_min((transformation*)&encode));
        assert(buffer_size(&mid) >= transformation_sink_min((transformation*)&encode));
        assert(buffer_size(&mid) >= transformation_source_min((transformation*)&decode));
        assert(buffer_size(&out) >= transformation_sink_min((transformation*)&decode));

        transformation_set_source(&in, (transformation*)&encode);
        transformation_set_sink(&mid, (transformation*)&encode);
        transformation_set_source(&mid, (transformation*)&decode);
        transformation_set_sink(&out, (transformation*)&decode);

        while (buffer_read_size(&in) >= transformation_source_min((transformation*)&encode) &&
               buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode))
        {
            if (!buffer_readable(&mid))
            {
                buffer_reset(&mid);
            }
            if (buffer_write_size(&mid) >= transformation_sink_min((transformation*)&encode))
            {
                transformation_transform((transformation*)&encode);
                HANDLE_EXCEPTION();
            }
            if (buffer_read_size(&mid) >= transformation_source_min((transformation*)&decode) &&
                buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode))
            {
                transformation_transform((transformation *) &decode);
                HANDLE_EXCEPTION();
            }
        }
        assert(buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode));
        while (buffer_write_size(&mid) < transformation_sink_min((transformation*)&encode) &&
               buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode))
        {
            if (!buffer_readable(&mid))
            {
                buffer_reset(&mid);
            }
            if (buffer_read_size(&mid) >= transformation_source_min((transformation*)&decode) &&
                buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode))
            {
                transformation_transform((transformation *) &decode);
                HANDLE_EXCEPTION();
            }
        }
        transformation_finalize((transformation*)&encode);
        HANDLE_EXCEPTION();
        assert(buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode));
        while (buffer_read_size(&mid) >= transformation_source_min((transformation*)&decode) &&
               buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode))
        {
            if (buffer_write_size(&mid) >= transformation_sink_min((transformation*)&encode))
            {
                transformation_transform((transformation*)&encode);
                HANDLE_EXCEPTION();
            }
            if (buffer_read_size(&mid) >= transformation_source_min((transformation*)&decode) &&
                buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode))
            {
                transformation_transform((transformation *) &decode);
                HANDLE_EXCEPTION();
            }
        }
        assert(buffer_write_size(&out) >= transformation_sink_min((transformation*)&decode));
        transformation_finalize((transformation*)&decode);
        HANDLE_EXCEPTION();
        assert(strcmp(msg, out.buf) == 0);
        assert(out.wpos == sizeof(msg));
        transformation_destructor((transformation*)&encode);
        transformation_destructor((transformation*)&decode);
        buffer_destructor(&in);
        buffer_destructor(&mid);
        buffer_destructor(&out);
    }
}
