add_library(buffer buffer.h buffer.c)
target_link_libraries(buffer CEasyException)

add_executable(buffer_test test.c)
target_link_libraries(buffer_test buffer)

add_test(buffer_test buffer_test)