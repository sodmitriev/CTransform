add_library(source_file source_file.h source_file.c)
target_link_libraries(source_file source)

add_library(sink_file sink_file.h sink_file.c)
target_link_libraries(sink_file sink)

add_executable(file_test test.c)
target_link_libraries(file_test source_file sink_file controller source_write sink_read)

add_test(file_test file_test)