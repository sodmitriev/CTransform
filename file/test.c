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

#include <assert.h>
#include <read_write/sink_read.h>
#include <read_write/source_write.h>
#include <controller.h>
#include <stdlib.h>
#include <zconf.h>

#include "source_file.h"
#include "sink_file.h"

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    fflush(stderr);                         \
    abort();                                \
}((void)(0))

int main()
{
    const int msg[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                       0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                       0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
                       0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
                       0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                       0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
                       0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                       0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
                       0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
                       0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
                       0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                       0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
                       0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
                       0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f};

    char buf[sizeof(msg) + 8];

    //Write to file
    {
        source_write in;
        sink_file out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        sink_file_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        source_write_set(msg, sizeof(int), sizeof(msg) / sizeof(int), &in);
        sink_file_open("test", &out);
        HANDLE_EXCEPTION();

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(source_write_get_result(&in) == sizeof(msg) / sizeof(*msg));

        sink_file_close(&out);
        HANDLE_EXCEPTION();

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        sink_destructor((sink *)&out);
    }

    //Read from file
    {
        source_file in;
        sink_read out;

        controller ctl;

        source_file_constructor(&in);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        source_file_open("test", &in);
        HANDLE_EXCEPTION();
        sink_read_set(buf, sizeof(int), sizeof(buf) / sizeof(int), &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(sink_read_get_result(&out) == sizeof(msg) / sizeof(*msg));
        assert(memcmp(buf, msg, sizeof(msg)) == 0);

        source_file_close(&in);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        sink_destructor((sink *)&out);

    }

    unlink("test");

    return 0;
}
