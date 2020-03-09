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

#include <CEasyException/exception.h>
#include <stdlib.h>
#include <read_write/source_write.h>
#include <file/sink_file.h>
#include <controller.h>

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    return 1;                               \
}((void)(0))

#define CHUNK_SIZE 4096

void next_chunk(char* ptr)
{
    memset(ptr, 1, CHUNK_SIZE);
}

bool more_chunks()
{
    static size_t cnt = 0;
    return cnt++ < 8;
}

int main(int argc, char** argv)
{

    EXCEPTION_CLEAR();

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <output file>",
                argv[0]);
        return 1;
    }

    char buffer[CHUNK_SIZE];

    source_write in;
    sink_file out;

    controller ctl;

    source_write_constructor(&in);
    HANDLE_EXCEPTION();
    sink_file_constructor(&out);
    HANDLE_EXCEPTION();

    controller_constructor(&ctl);
    HANDLE_EXCEPTION();

    sink_file_open(argv[1], &out);
    HANDLE_EXCEPTION();

    controller_set_source((source *)&in, &ctl);
    controller_set_sink((sink *)&out, &ctl);

    while(more_chunks())
    {
        next_chunk(buffer);
        source_write_set(buffer, 1, CHUNK_SIZE, &in);
        controller_work(&ctl);
        HANDLE_EXCEPTION();
    }

    controller_finalize(&ctl);
    HANDLE_EXCEPTION();

    printf("File is written!");

    controller_destructor(&ctl);

    source_destructor((source *)&in);
    sink_destructor((sink *)&out);

    return 0;
}
