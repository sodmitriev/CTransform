add_library(sink_read sink_read.c sink_read.h)
target_link_libraries(sink_read sink)

add_library(source_write source_write.c source_write.h)
target_link_libraries(source_write source)

add_executable(read_write_test test.c)
target_link_libraries(read_write_test sink_read source_write)