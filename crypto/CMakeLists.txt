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

add_library(transformation_encrypt transformation_encrypt.h transformation_encrypt.c)
target_include_directories(transformation_encrypt PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_encrypt transformation ${OPENSSL_LIBRARIES})

add_library(transformation_decrypt transformation_decrypt.h transformation_decrypt.c)
target_include_directories(transformation_decrypt PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_decrypt transformation ${OPENSSL_LIBRARIES})

add_library(transformation_hash transformation_hash.h transformation_hash.c)
target_include_directories(transformation_hash PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_hash transformation ${OPENSSL_LIBRARIES})

add_executable(transformation_crypto_test test.c)
target_link_libraries(transformation_crypto_test transformation_encrypt transformation_decrypt transformation_hash
                      controller sink_read source_write)

add_test(transformation_crypto_test transformation_crypto_test)