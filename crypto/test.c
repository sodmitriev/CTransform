#include <controller.h>
#include <read_write/source_write.h>
#include <read_write/sink_read.h>
#include <CEasyException/exception.h>
#include "transformation_encrypt.h"
#include "transformation_decrypt.h"
#include "transformation_hash.h"
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

    //Pre calculated hash value for msg using sha1
    const unsigned char hash_val[] = {0x5e, 0x91, 0x6a, 0x59, 0xe9, 0x97, 0x3b, 0xb7, 0xbe, 0x02, 0x90, 0xe1, 0xf2,
                                      0x83, 0xe3, 0xd3, 0x08, 0xaa, 0x7c, 0x11};

    EXCEPTION_CLEAR();

    //Encrypt -> Decrypt
    {
        char buf[sizeof(msg) + 8];

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

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&encrypt, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&decrypt, &ctl);
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
        transformation_destructor((transformation *)&encrypt);
        transformation_destructor((transformation *)&decrypt);
        sink_destructor((sink *)&out);
    }

    //Hash
    {
        char buf[sizeof(hash_val) + 8];

        source_write in;
        transformation_hash hash;
        sink_read out;

        controller ctl;

        source_write_constructor(&in);
        HANDLE_EXCEPTION();
        transformation_hash_constructor("sha1", &hash);
        HANDLE_EXCEPTION();
        sink_read_constructor(&out);
        HANDLE_EXCEPTION();

        controller_constructor(&ctl);
        HANDLE_EXCEPTION();

        controller_set_source((source *)&in, &ctl);
        HANDLE_EXCEPTION();
        controller_add_transformation((transformation *)&hash, &ctl);
        HANDLE_EXCEPTION();
        controller_set_sink((sink *)&out, &ctl);
        HANDLE_EXCEPTION();

        //sizeof(msg) - 1 to avoid hashing '\0'
        source_write_set(msg, 1, sizeof(msg) - 1, &in);
        sink_read_set(buf, 1, sizeof(buf), &out);

        controller_finalize(&ctl);
        HANDLE_EXCEPTION();

        assert(sink_read_get_result(&out) == sizeof(hash_val));
        assert(memcmp(buf, hash_val, sizeof(hash_val)) == 0);

        controller_destructor(&ctl);
        source_destructor((source *)&in);
        transformation_destructor((transformation *)&hash);
        sink_destructor((sink *)&out);
    }

    return 0;
}
