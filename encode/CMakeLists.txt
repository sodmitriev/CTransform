find_package(OpenSSL REQUIRED)

add_library(transformation_b64_encode transformation_b64_encode.h transformation_b64_encode.c)
target_include_directories(transformation_encrypt PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_b64_encode transformation ${OPENSSL_LIBRARIES})

add_library(transformation_b64_decode transformation_b64_decode.h transformation_b64_decode.c)
target_include_directories(transformation_encrypt PUBLIC ${OPENSSL_INCLUDE_DIR})
target_link_libraries(transformation_b64_decode transformation ${OPENSSL_LIBRARIES})

add_executable(transformation_b64_test test.c)
target_link_libraries(transformation_b64_test transformation_b64_encode transformation_b64_decode
                      controller source_write sink_read)

add_test(transformation_b64_test transformation_b64_test)