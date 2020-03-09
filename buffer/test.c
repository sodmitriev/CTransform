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

#include "buffer.h"
#include <CEasyException/exception.h>
#include <assert.h>
#include <stdlib.h>

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    fflush(stderr);                         \
    abort();                                \
}((void)(0))

#define BUF_SIZE 16

int main()
{
    char msg[BUF_SIZE];
    buffer buf;

    EXCEPTION_CLEAR();

    for(size_t i = 0; i < sizeof(msg); ++i)
    {
        msg[i] = (char)i;
    }
    buffer_constructor(BUF_SIZE, &buf);
    HANDLE_EXCEPTION();

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE);
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == 0);

    memcpy(buffer_wpos(&buf), msg, BUF_SIZE / 2);
    buffer_winc(BUF_SIZE / 2, &buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE - (BUF_SIZE / 2));
    assert(buffer_read_size(&buf) == BUF_SIZE / 2);
    assert(buffer_occupied(&buf) == BUF_SIZE / 2);

    assert(memcmp(msg, buffer_rpos(&buf), BUF_SIZE / 2) == 0);
    buffer_rinc(BUF_SIZE / 2, &buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE - (BUF_SIZE / 2));
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == BUF_SIZE / 2);

    memcpy(buffer_wpos(&buf), msg + (BUF_SIZE / 2), BUF_SIZE - (BUF_SIZE / 2));
    buffer_winc(BUF_SIZE - (BUF_SIZE / 2), &buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(!buffer_writable(&buf));
    assert(buffer_readable(&buf));
    assert(buffer_write_size(&buf) == 0);
    assert(buffer_read_size(&buf) == BUF_SIZE - (BUF_SIZE / 2));
    assert(buffer_occupied(&buf) == BUF_SIZE);

    assert(memcmp(msg + (BUF_SIZE / 2), buffer_rpos(&buf), BUF_SIZE - (BUF_SIZE / 2)) == 0);
    buffer_rinc(BUF_SIZE - (BUF_SIZE / 2), &buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(!buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == 0);
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == BUF_SIZE);

    buffer_resize(BUF_SIZE * 2, &buf);
    HANDLE_EXCEPTION();

    assert(buffer_size(&buf) == BUF_SIZE * 2);
    assert(buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE);
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == BUF_SIZE);

    buffer_resize(BUF_SIZE / 2, &buf);
    HANDLE_EXCEPTION();

    assert(buffer_size(&buf) == BUF_SIZE / 2);
    assert(!buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == 0);
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == BUF_SIZE / 2);

    buffer_resize(BUF_SIZE, &buf);
    HANDLE_EXCEPTION();

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE - (BUF_SIZE / 2));
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == BUF_SIZE / 2);

    memcpy(buffer_wpos(&buf), msg + (BUF_SIZE / 2), BUF_SIZE - (BUF_SIZE / 2));
    buffer_winc(BUF_SIZE - (BUF_SIZE / 2), &buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(!buffer_writable(&buf));
    assert(buffer_readable(&buf));
    assert(buffer_write_size(&buf) == 0);
    assert(buffer_read_size(&buf) == BUF_SIZE - (BUF_SIZE / 2));
    assert(buffer_occupied(&buf) == BUF_SIZE);

    buffer_compact(&buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE / 2);
    assert(buffer_read_size(&buf) == BUF_SIZE - (BUF_SIZE / 2));
    assert(buffer_occupied(&buf) == BUF_SIZE / 2);

    assert(memcmp(msg + (BUF_SIZE / 2), buffer_rpos(&buf), BUF_SIZE - (BUF_SIZE / 2)) == 0);
    buffer_rinc(BUF_SIZE - (BUF_SIZE / 2), &buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE / 2);
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == BUF_SIZE / 2);

    buffer_reset(&buf);

    assert(buffer_size(&buf) == BUF_SIZE);
    assert(buffer_writable(&buf));
    assert(!buffer_readable(&buf));
    assert(buffer_write_size(&buf) == BUF_SIZE);
    assert(buffer_read_size(&buf) == 0);
    assert(buffer_occupied(&buf) == 0);


    buffer_destructor(&buf);

    return 0;
}
