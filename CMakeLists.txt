# Copyright (C) 2020 Sviatoslav Dmitriev
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.15)
project(ctransform C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wenum-compare -Wredundant-decls\
                   -Wno-sign-conversion ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-g -O0 ${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG ${CMAKE_C_FLAGS_RELEASE}")

enable_testing()

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
add_subdirectory(file)
add_subdirectory(compress)

add_executable(controller_test controller_test.c)
target_link_libraries(controller_test controller transformation_encrypt transformation_decrypt sink_read source_write)

add_test(controller_test controller_test)