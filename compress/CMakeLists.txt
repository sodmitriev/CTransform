FIND_PACKAGE(ZLIB REQUIRED)

add_library(transformation_compress transformation_compress.h transformation_compress.c)
target_include_directories(transformation_compress PRIVATE ${ZLIB_INCLUDE_DIRS})
target_link_libraries(transformation_compress transformation ${ZLIB_LIBRARIES})

add_library(transformation_decompress transformation_decompress.h transformation_decompress.c)
target_include_directories(transformation_decompress PRIVATE ${ZLIB_INCLUDE_DIRS})
target_link_libraries(transformation_decompress transformation ${ZLIB_LIBRARIES})

add_executable(transformation_compress_test test.c)
target_link_libraries(transformation_compress_test transformation_compress transformation_decompress
                      source_write sink_read controller source_file sink_file)