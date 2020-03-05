add_library(sink_read sink_read.c sink_read.h)
target_link_libraries(sink_read sink)

add_library(source_write source_write.c source_write.h)
target_link_libraries(source_write source)

add_library(sink_getc sink_getc.h sink_getc.c)
target_link_libraries(sink_getc sink)

add_library(sink_gets sink_gets.h sink_gets.c)
target_link_libraries(sink_gets sink)

add_executable(read_write_test test.c)
target_link_libraries(read_write_test sink_read source_write sink_getc sink_gets controller)

add_test(read_write_test read_write_test)