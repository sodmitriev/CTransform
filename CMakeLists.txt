cmake_minimum_required(VERSION 3.15)
project(ctransform C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wenum-compare -Wredundant-decls\
                   -Wno-sign-conversion ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 ${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG ${CMAKE_C_FLAGS_RELEASE}")

add_subdirectory(CEasyException)
include_directories(.)

add_subdirectory(buffer)

add_library(sink sink.h sink.c)
target_link_libraries(sink buffer)

add_library(source source.h source.c)
target_link_libraries(source buffer)

add_library(transformation transformation.h transformation.c)
target_link_libraries(transformation buffer)

add_library(controller controller.h controller.c)
target_link_libraries(controller sink source transformation)

add_subdirectory(read_write)
add_subdirectory(util)
add_subdirectory(crypto)
add_subdirectory(encode)

add_executable(controller_test controller_test.c)
target_link_libraries(controller_test controller transformation_encrypt transformation_decrypt sink_read source_write)