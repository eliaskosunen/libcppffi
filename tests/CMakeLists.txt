file(GLOB sources_tests *.cpp)

add_executable(tests ${sources_tests})
target_link_libraries(tests ffi)
add_test(NAME libcppffi COMMAND tests)
