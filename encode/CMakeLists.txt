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

find_package(OpenSSL REQUIRED)

add_library(transformation_b64_encode transformation_b64_encode.h transformation_b64_encode.c)
target_include_directories(transformation_encrypt PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_b64_encode transformation ${OPENSSL_LIBRARIES})

add_library(transformation_b64_decode transformation_b64_decode.h transformation_b64_decode.c)
target_include_directories(transformation_encrypt PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_b64_decode transformation ${OPENSSL_LIBRARIES})

add_executable(transformation_b64_test test.c)
target_link_libraries(transformation_b64_test transformation_b64_encode transformation_b64_decode
                      controller source_write sink_read)

add_test(transformation_b64_test transformation_b64_test)