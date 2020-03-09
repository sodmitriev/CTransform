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