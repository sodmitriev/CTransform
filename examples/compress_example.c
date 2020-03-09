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
#include <file/source_file.h>
#include <file/sink_file.h>
#include <compress/transformation_compress.h>
#include <compress/transformation_decompress.h>
#include <controller.h>

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    return 1;                               \
}((void)(0))

int main(int argc, char** argv)
{

    EXCEPTION_CLEAR();

    if(argc != 5)
    {
        fprintf(stderr, "Usage: %s <compress/decompress> <input file> <output file> <compression level>",
                argv[0]);
        return 1;
    }
    char* end_ptr;
    long level = strtol(argv[4], &end_ptr, 10);
    if(end_ptr == argv[4] || level < 0 || level > 10)
    {
        fprintf(stderr, "Level must be a number from 0 to 10");
        return 1;
    }

    bool op_compress;
    if(strcmp(argv[1], "compress") == 0)
    {
        op_compress = true;
    }
    else if(strcmp(argv[1], "decompress") == 0)
    {
        op_compress = false;
    }
    else
    {
        fprintf(stderr, "Only \"compress\" and \"decompress\" operations are supported");
        return 1;
    }

    source_file in;
    transformation_compress compress;
    transformation_decompress decompress;
    sink_file out;

    controller ctl;

    source_file_constructor(&in);
    HANDLE_EXCEPTION();
    if(op_compress)
    {
        transformation_compress_constructor((int)level, &compress);
        HANDLE_EXCEPTION();
    }
    else
    {
        transformation_decompress_constructor(&decompress);
        HANDLE_EXCEPTION();
    }
    sink_file_constructor(&out);
    HANDLE_EXCEPTION();

    controller_constructor(&ctl);
    HANDLE_EXCEPTION();

    source_file_open(argv[2], &in);
    HANDLE_EXCEPTION();
    sink_file_open(argv[3], &out);
    HANDLE_EXCEPTION();

    controller_set_source((source *)&in, &ctl);
    controller_add_transformation(op_compress ? (transformation *)&compress : (transformation *)&decompress, &ctl);
    controller_set_sink((sink *)&out, &ctl);

    controller_finalize(&ctl);
    HANDLE_EXCEPTION();

    printf("File is %s!", op_compress ? "compressed" : "decompressed");

    controller_destructor(&ctl);

    source_destructor((source *)&in);
    transformation_destructor(op_compress ? (transformation *)&compress : (transformation *)&decompress);
    sink_destructor((sink *)&out);

    return 0;
}
