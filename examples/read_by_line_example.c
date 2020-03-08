#include <CEasyException/exception.h>
#include <read_write/sink_gets.h>
#include <file/source_file.h>
#include <controller.h>
#include <limits.h>

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    return 1;                               \
}((void)(0))

int main(int argc, char** argv)
{

    EXCEPTION_CLEAR();

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <input file>",
                argv[0]);
        return 1;
    }

    char buffer[LINE_MAX];

    source_file in;
    sink_gets out;

    controller ctl;

    source_file_constructor(&in);
    HANDLE_EXCEPTION();
    sink_gets_constructor(&out);
    HANDLE_EXCEPTION();

    controller_constructor(&ctl);
    HANDLE_EXCEPTION();

    source_file_open(argv[1], &in);
    HANDLE_EXCEPTION();

    controller_set_source((source *)&in, &ctl);
    controller_set_sink((sink *)&out, &ctl);

    while(controller_get_stage(&ctl) != controller_stage_done)
    {
        sink_gets_set(buffer, LINE_MAX, &out);
        controller_finalize(&ctl);
        HANDLE_EXCEPTION();
        printf("%s", buffer);
    }

    controller_destructor(&ctl);

    source_destructor((source *)&in);
    sink_destructor((sink *)&out);

    return 0;
}
