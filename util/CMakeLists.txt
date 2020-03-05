add_library(transformation_replace transformation_replace.h transformation_replace.c)
target_link_libraries(transformation_replace transformation)

add_library(transformation_remove transformation_remove.h transformation_remove.c)
target_link_libraries(transformation_remove transformation)

add_executable(transformation_util_test test.c)
target_link_libraries(transformation_util_test transformation_replace transformation_remove
                      controller source_write sink_read)