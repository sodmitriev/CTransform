#include <controller.h>
#include <read_write/source_write.h>
#include <read_write/sink_read.h>
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
        char buf[sizeof(msg) + 8];

        source_write in;
        transformation_replace replace;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_replace_constructor("0267", 4, ' ', &replace);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&replace, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        source_write_set(msg, 1, sizeof(msg), &in);
        sink_read_set(buf, 1, sizeof(buf), &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(sink_read_get_result(&out) == sizeof(msg));
        for(size_t i = 0; i < sizeof(msg); ++i)
        {
            if(msg[i] == '0' || msg[i] == '2' || msg[i] == '6' || msg[i] == '7')
            {
                assert(buf[i] == ' ');
            }
            else
            {
                assert(buf[i] == msg[i]);
            }
        }

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&replace);
        sink_destructor((sink *)&out);
    }

    //Remove
    {
        char buf[sizeof(msg) + 8];

        source_write in;
        transformation_remove remove;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_remove_constructor("0267", 4, &remove);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&remove, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        source_write_set(msg, 1, sizeof(msg), &in);
        sink_read_set(buf, 1, sizeof(buf), &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(sink_read_get_result(&out) == sizeof(msg) - 64);
        size_t out_i = 0;
        for(size_t i = 0; i < sizeof(msg); ++i)
        {
            if(msg[i] != '0' && msg[i] != '2' && msg[i] != '6' && msg[i] != '7')
            {
                assert(buf[out_i] == msg[i]);
                ++out_i;
            }
        }

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&remove);
        sink_destructor((sink *)&out);
    }

    return 0;
}
