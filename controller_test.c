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

#include "controller.h"
#include <read_write/sink_read.h>
#include <read_write/source_write.h>
#include <crypto/transformation_encrypt.h>
#include <crypto/transformation_decrypt.h>
#include <stdlib.h>
#include <assert.h>

#define HANDLE_EXCEPTION()                  \
if(EXCEPTION_IS_THROWN)                     \
{                                           \
    fprintf(stderr, "%s", EXCEPTION_MSG);   \
    fflush(stderr);                         \
    abort();                                \
}((void)(0))

//Add +1 because of final not full chunk
#define CHUNK_NUM 16

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

    //Write and read whole message
    {

        char buf[sizeof(msg)];

        source_write in;
        transformation_encrypt encrypt;
        transformation_decrypt decrypt;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_encrypt_constructor("aes-256-cbc", "sha1", "mykey", &encrypt);
        HANDLE_EXCEPTION();
        transformation_decrypt_constructor("aes-256-cbc", "sha1", "mykey", &decrypt);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_build);

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&encrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_build);

        source_write_set(msg, 1, sizeof(msg), &in);
        sink_read_set(buf, 1, sizeof(buf), &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_done);

        assert(sink_read_get_result(&out) == sizeof(msg));
        assert(strcmp(buf, msg) == 0);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&encrypt);
        transformation_destructor((transformation *)&decrypt);
        sink_destructor((sink *)&out);

    }

    //Write whole, read by chunk
    {
        size_t chunk_size = sizeof(msg) / CHUNK_NUM;
        char buf[sizeof(msg)];

        source_write in;
        transformation_encrypt encrypt;
        transformation_decrypt decrypt;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_encrypt_constructor("aes-256-cbc", "sha1", "mykey", &encrypt);
        HANDLE_EXCEPTION();
        transformation_decrypt_constructor("aes-256-cbc", "sha1", "mykey", &decrypt);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_build);

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&encrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_build);

        source_write_set(msg, 1, sizeof(msg), &in);

        size_t chunk = 0;

        while(chunk < CHUNK_NUM)
        {
            sink_read_set(buf + chunk * chunk_size, 1, chunk_size, &out);

            controller_finalize(&ctl);
            HANDLE_EXCEPTION();

            assert(controller_get_stage(&ctl) == controller_stage_final);

            assert(sink_end((sink *)&out));

            assert(sink_read_get_result(&out) == chunk_size);

            ++chunk;
        }

        sink_read_set(buf + chunk * chunk_size, 1, sizeof(msg) % chunk_size, &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_done);

        assert(source_end((source *)&in));
        assert(sink_end((sink *)&out));
        assert(sink_read_get_result(&out) == sizeof(msg) % chunk_size);
        assert(memcmp(buf, msg, sizeof(msg)) == 0);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&encrypt);
        transformation_destructor((transformation *)&decrypt);
        sink_destructor((sink *)&out);
    }

    //Write by chunk, read whole
    {
        size_t chunk_size = sizeof(msg) / CHUNK_NUM;
        char buf[sizeof(msg)];

        source_write in;
        transformation_encrypt encrypt;
        transformation_decrypt decrypt;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_encrypt_constructor("aes-256-cbc", "sha1", "mykey", &encrypt);
        HANDLE_EXCEPTION();
        transformation_decrypt_constructor("aes-256-cbc", "sha1", "mykey", &decrypt);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_build);

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&encrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_build);

        sink_read_set(buf, 1, sizeof(msg), &out);

        size_t chunk = 0;

        while(chunk < CHUNK_NUM)
        {
            source_write_set(msg + chunk * chunk_size, 1, chunk_size, &in);

            controller_work(&ctl);
            HANDLE_EXCEPTION();

            assert(controller_get_stage(&ctl) == controller_stage_work);

            assert(source_end((source *)&in));
            assert(!sink_end((sink *)&out));

            ++chunk;
        }

        source_write_set(msg + chunk * chunk_size, 1, sizeof(msg) % chunk_size, &in);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(controller_get_stage(&ctl) == controller_stage_done);

        assert(source_end((source *)&in));
        assert(sink_end((sink *)&out));
        assert(sink_read_get_result(&out) == sizeof(msg));
        assert(memcmp(buf, msg, sizeof(msg)) == 0);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&encrypt);
        transformation_destructor((transformation *)&decrypt);
        sink_destructor((sink *)&out);
    }

    return 0;
}
