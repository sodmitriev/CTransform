#include <controller.h>
#include <read_write/source_write.h>
#include <read_write/sink_read.h>
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

    //Base64
    {
        char buf[sizeof(msg) + 8];

        source_write in;
        transformation_b64_encode encode;
        transformation_b64_decode decode;
        sink_read out;

        controller ctl;

        source_write_constructor(msg, 1, sizeof(msg), &in);
        HANDLE_EXCEPTION();
        transformation_b64_encode_constructor(&encode);
        HANDLE_EXCEPTION();
        transformation_b64_decode_constructor(&decode);
        HANDLE_EXCEPTION();
        sink_read_constructor(buf, 1, sizeof(buf), &out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&encode, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decode, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(sink_read_get_result(&out) == sizeof(msg));
        assert(strcmp(buf, msg) == 0);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&encode);
        transformation_destructor((transformation *)&decode);
        sink_destructor((sink *)&out);
    }

}
