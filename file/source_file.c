#include "source_file.h"
#include <string.h>
#include <assert.h>
#include <CEasyException/exception.h>
#include <errno.h>

source_call_tab source_call_tab_file =
        {
                .destructor  = (void (*)(source *))source_file_destructor,
                .send        = (void (*)(source *))source_file_send,
                .sink_min    = (size_t (*)(const source *))source_file_sink_min,
                .end         = (bool (*)(const source *))source_file_end
        };

void source_file_constructor(source_file *this)
{
    this->file = NULL;
    this->base.call_tab = &source_call_tab_file;
    this->base.sink = NULL;
}

void source_file_destructor(source_file *this)
{
    if(this->file)
    {
        fclose(this->file);
    }
    this->base.sink = NULL;
    this->file = NULL;
}

void source_file_send(source_file *this)
{
    assert(!ferror(this->file));
    assert(!feof(this->file));
    size_t cnt = fread(buffer_wpos(this->base.sink), 1, buffer_write_size(this->base.sink), this->file);
    buffer_winc(cnt, this->base.sink);
    if(ferror(this->file))
    {
        int tmp = errno;
        EXCEPTION_THROW(tmp, "%s", "Failed to read from file");
        clearerr(this->file);
        return;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
size_t source_file_sink_min(source_file *this)
{
    return 1;
}
#pragma GCC diagnostic pop

bool source_file_end(const source_file *this)
{
    return feof(this->file);
}

extern void source_file_open(const char *pathname, source_file *this)
{
    assert(!this->file);
    this->file = fopen(pathname, "r");
    if(!this->file)
    {
        int tmp = errno;
        EXCEPTION_THROW(tmp, "Failed to open file \"%s\"", pathname);
        return;
    }
}

extern void source_file_close(source_file *this)
{
    assert(this->file);
    int err = fclose(this->file);
    if(err < 0)
    {
        int tmp = errno;
        EXCEPTION_THROW(tmp, "%s", "Failed to close file");
    }
    this->file = NULL;
}
