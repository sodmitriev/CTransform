#include <buffer/buffer.h>
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
    const unsigned char hash_val[] = {0x5e, 0x91, 0x6a, 0x59, 0xe9, 0x97, 0x3b, 0xb7, 0xbe, 0x02, 0x90, 0xe1, 0xf2, 0x83, 0xe3, 0xd3, 0x08, 0xaa, 0x7c, 0x11};

    EXCEPTION_CLEAR();

    //Encrypt -> Decrypt
    {
        buffer in;
        buffer mid;
        buffer out;
        transformation_encrypt encrypt;
        transformation_decrypt decrypt;

        buffer_constructor(sizeof(msg), &in);
        HANDLE_EXCEPTION();
        buffer_constructor(64, &mid);
        HANDLE_EXCEPTION();
        buffer_constructor(sizeof(msg) + 128, &out);
        HANDLE_EXCEPTION();

        memcpy(in.buf, msg, sizeof(msg));
        in.wpos = sizeof(msg);


        transformation_encrypt_constructor("aes-256-cbc", "sha1", "mykey", &encrypt);
        HANDLE_EXCEPTION();
        transformation_decrypt_constructor("aes-256-cbc", "sha1", "mykey", &decrypt);
        HANDLE_EXCEPTION();

        assert(buffer_size(&in) >= transformation_source_min((transformation*)&encrypt));
        assert(buffer_size(&mid) >= transformation_sink_min((transformation*)&encrypt));
        assert(buffer_size(&mid) >= transformation_source_min((transformation*)&decrypt));
        assert(buffer_size(&out) >= transformation_sink_min((transformation*)&decrypt));

        transformation_set_source(&in, (transformation*)&encrypt);
        transformation_set_sink(&mid, (transformation*)&encrypt);
        transformation_set_source(&mid, (transformation*)&decrypt);
        transformation_set_sink(&out, (transformation*)&decrypt);

        while ((buffer_readable(&in) || buffer_readable(&mid)) && buffer_writable(&out))
        {
            if (!buffer_readable(&in))
            {
                buffer_reset(&in);
            }
            if (!buffer_readable(&mid))
            {
                buffer_reset(&mid);
            }
            if (buffer_write_size(&mid) >= transformation_sink_min((transformation*)&encrypt))
            {
                transformation_transform((transformation*)&encrypt);
                HANDLE_EXCEPTION();
            }
            assert(buffer_write_size(&out) >= transformation_sink_min((transformation*)&decrypt));
            transformation_transform((transformation*)&decrypt);
            HANDLE_EXCEPTION();
        }
        assert(buffer_write_size(&mid) >= transformation_sink_min((transformation*)&encrypt));
        transformation_finalize((transformation*)&encrypt);
        HANDLE_EXCEPTION();
        while (buffer_readable(&mid) && buffer_writable(&out))
        {
            if (!buffer_readable(&mid))
            {
                buffer_reset(&mid);
            }
            assert(buffer_write_size(&out) >= transformation_sink_min((transformation*)&decrypt));
            transformation_transform((transformation*)&decrypt);
            HANDLE_EXCEPTION();
        }
        transformation_finalize((transformation*)&decrypt);
        HANDLE_EXCEPTION();
        assert(strcmp(msg, out.buf) == 0);
        assert(out.wpos == sizeof(msg));
        transformation_destructor((transformation*)&encrypt);
        transformation_destructor((transformation*)&decrypt);
        buffer_destructor(&in);
        buffer_destructor(&mid);
        buffer_destructor(&out);
    }

    //Hash
    {
        buffer in;
        buffer out;
        transformation_hash hash;

        buffer_constructor(sizeof(msg), &in);
        HANDLE_EXCEPTION();
        buffer_constructor(64, &out);
        HANDLE_EXCEPTION();

        memcpy(buffer_wpos(&in), msg, sizeof(msg) - 1);
        buffer_winc(sizeof(msg) - 1, &in);

        transformation_hash_constructor("sha1", &hash);
        HANDLE_EXCEPTION();

        transformation_set_source(&in, (transformation*)&hash);
        transformation_set_sink(&out, (transformation*)&hash);

        while(buffer_readable(&in))
        {
            assert(buffer_size(&in) >= transformation_source_min((transformation*)&hash));
            assert(buffer_size(&out) >= transformation_sink_min((transformation*)&hash));
            transformation_transform((transformation*)&hash);
            HANDLE_EXCEPTION();
        }

        assert(buffer_size(&out) >= transformation_sink_min((transformation*)&hash));

        transformation_finalize((transformation*)&hash);
        HANDLE_EXCEPTION();

        assert(buffer_read_size(&out) == sizeof(hash_val));
        assert(memcmp(buffer_rpos(&out), hash_val, sizeof(hash_val)) == 0);

        transformation_destructor((transformation*)&hash);
        buffer_destructor(&in);
        buffer_destructor(&out);
    }

    return 0;
}
