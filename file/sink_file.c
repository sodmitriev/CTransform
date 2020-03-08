#include "sink_file.h"
#include <string.h>
#include <assert.h>
#include <CEasyException/exception.h>
#include <errno.h>

sink_call_tab sink_call_tab_file =
        {
                .destructor  = (void (*)(sink *))sink_file_destructor,
                .send        = (void (*)(sink *))sink_file_send,
                .source_min  = (size_t (*)(const sink *))sink_file_sink_min,
                .end         = (bool (*)(const sink *))sink_file_end
        };

void sink_file_constructor(sink_file *this)
{
    this->file = NULL;
    this->base.call_tab = &sink_call_tab_file;
    this->base.source = NULL;
}

void sink_file_destructor(sink_file *this)
{
    if(this->file)
    {
        fclose(this->file);
    }
    this->base.source = NULL;
    this->file = NULL;
}

void sink_file_send(sink_file *this)
{
    assert(!ferror(this->file));
    assert(!feof(this->file));
    size_t cnt = fwrite(buffer_rpos(this->base.source), 1, buffer_read_size(this->base.source), this->file);
    buffer_rinc(cnt, this->base.source);
    if(ferror(this->file))
    {
        int tmp = errno;
        EXCEPTION_THROW(tmp, "%s", "Failed to write to file");
        clearerr(this->file);
        return;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
size_t sink_file_sink_min(const sink_file *this)
{
    return 1;
}
#pragma GCC diagnostic pop

bool sink_file_end(const sink_file *this)
{
    return feof(this->file);
}

extern void sink_file_open(const char *pathname, sink_file *this)
{
    assert(!this->file);
    this->file = fopen(pathname, "w");
    if(!this->file)
    {
        int tmp = errno;
        EXCEPTION_THROW(tmp, "Failed to open file \"%s\"", pathname);
        return;
    }
}

extern void sink_file_close(sink_file *this)
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
