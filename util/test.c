#include "transformation_replace.h"
#include "transformation_remove.h"
#include <stdlib.h>
#include <CEasyException/exception.h>
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

    //Replace
    {
        buffer in;
        buffer out;
        transformation_replace replace;

        buffer_constructor(sizeof(msg), &in);
        HANDLE_EXCEPTION();

        buffer_constructor(sizeof(msg) + 1, &out);
        HANDLE_EXCEPTION();

        memcpy(buffer_wpos(&in), msg, sizeof(msg));
        buffer_winc(sizeof(msg), &in);

        transformation_replace_constructor("0267", 4, ' ', &replace);
        HANDLE_EXCEPTION();

        transformation_set_source(&in, (transformation*)&replace);
        transformation_set_sink(&out, (transformation*)&replace);

        while(buffer_readable(&in))
        {
            transformation_transform((transformation*)&replace);
            HANDLE_EXCEPTION();
        }
        transformation_finalize((transformation*)&replace);
        HANDLE_EXCEPTION();
        assert(sizeof(msg) == buffer_read_size(&out));
        for(size_t i = 0; i < sizeof(msg); ++i)
        {
            if(msg[i] == '0' || msg[i] == '2' || msg[i] == '6' || msg[i] == '7')
            {
                assert(buffer_rpos(&out)[i] == ' ');
            }
            else
            {
                assert(buffer_rpos(&out)[i] == msg[i]);
            }
        }

        transformation_destructor((transformation*)&replace);
        buffer_destructor(&in);
        buffer_destructor(&out);
    }

    //Remove
    {
        buffer in;
        buffer out;
        transformation_remove remove;

        buffer_constructor(sizeof(msg), &in);
        HANDLE_EXCEPTION();

        buffer_constructor(sizeof(msg) + 1, &out);
        HANDLE_EXCEPTION();

        memcpy(buffer_wpos(&in), msg, sizeof(msg));
        buffer_winc(sizeof(msg), &in);

        transformation_remove_constructor("0267", 4, &remove);
        HANDLE_EXCEPTION();

        transformation_set_source(&in, (transformation*)&remove);
        transformation_set_sink(&out, (transformation*)&remove);

        while(buffer_readable(&in))
        {
            transformation_transform((transformation*)&remove);
            HANDLE_EXCEPTION();
        }
        transformation_finalize((transformation*)&remove);
        HANDLE_EXCEPTION();
        assert(sizeof(msg) - 64 == buffer_read_size(&out));
        size_t out_i = 0;
        for(size_t i = 0; i < sizeof(msg); ++i)
        {
            if(msg[i] != '0' && msg[i] != '2' && msg[i] != '6' && msg[i] != '7')
            {
                assert(buffer_rpos(&out)[out_i] == msg[i]);
                ++out_i;
            }
        }

        transformation_destructor((transformation*)&remove);
        buffer_destructor(&in);
        buffer_destructor(&out);
    }

    return 0;
}
