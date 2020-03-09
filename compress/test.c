/*
 * Copyright (C) 2020 Sviatoslav Dmitriev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <controller.h>
#include <CEasyException/exception.h>
#include "transformation_compress.h"
#include "transformation_decompress.h"
#include <stdlib.h>

#ifndef COMPRESSION_TEST_LONG
#include <assert.h>
#include <read_write/source_write.h>
#include <read_write/sink_read.h>
#else
#include <file/source_file.h>
#include <file/sink_file.h>
#include <zconf.h>
#endif

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    fflush(stderr);                         \
    abort();                                \
}((void)(0))

int main()
{

    EXCEPTION_CLEAR();

#ifndef COMPRESSION_TEST_LONG

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

    //Compress -> Decompress
    {
        char buf[sizeof(msg) + 8];

        source_write in;
        transformation_compress compress;
        transformation_decompress decompress;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_compress_constructor(5, &compress);
        HANDLE_EXCEPTION();
        transformation_decompress_constructor(&decompress);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&compress, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decompress, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        source_write_set(msg, 1, sizeof(msg), &in);
        sink_read_set(buf, 1, sizeof(buf), &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(sink_read_get_result(&out) == sizeof(msg));
        assert(strcmp(buf, msg) == 0);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&compress);
        transformation_destructor((transformation *)&decompress);
        sink_destructor((sink *)&out);
    }

#else

    //Compress and decompress 4GiB of random data
    {
        printf("Copying...\n");
        fflush(stdout);
        system("dd if=/dev/urandom of=testfile bs=4096 count=1048576");
        printf("Compressing --> decompressing...\n");
        fflush(stdout);

        source_file in;
        transformation_compress compress;
        transformation_decompress decompress;
        sink_file out;

        controller ctl;

        source_file_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_compress_constructor(5, &compress);
        HANDLE_EXCEPTION();
        transformation_decompress_constructor(&decompress);
        HANDLE_EXCEPTION();
        sink_file_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&compress, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decompress, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        source_file_open("testfile", &in);
        sink_file_open("testfile.res", &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&compress);
        transformation_destructor((transformation *)&decompress);
        sink_destructor((sink *)&out);

        printf("Comparing results...\n");
        fflush(stdout);

        if(system("diff testfile testfile.res") != 0)
        {
            abort();
        }

        unlink("testfile");
        unlink("testfile.res");

    }

#endif

    return 0;
}
