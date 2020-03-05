find_package(OpenSSL REQUIRED)

add_library(transformation_encrypt transformation_encrypt.h transformation_encrypt.c)
target_include_directories(transformation_encrypt PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_encrypt transformation ${OPENSSL_LIBRARIES})

add_library(transformation_decrypt transformation_decrypt.h transformation_decrypt.c)
target_include_directories(transformation_decrypt PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_decrypt transformation ${OPENSSL_LIBRARIES})

add_library(transformation_hash transformation_hash.h transformation_hash.c)
target_include_directories(transformation_hash PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_hash transformation ${OPENSSL_LIBRARIES})

add_executable(transformation_crypto_test test.c)
target_link_libraries(transformation_crypto_test transformation_encrypt transformation_decrypt transformation_hash
                      controller sink_read source_write)

add_test(transformation_crypto_test transformation_crypto_test)