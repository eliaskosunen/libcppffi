file(GLOB sources_tests *.cpp)

add_executable(tests ${sources_tests})
target_link_libraries(tests ffi)
add_test(NAME libcppffi COMMAND tests)

if(COVERALLS)
    file(GLOB_RECURSE coveralls_sources ${PROJECT_SOURCE_DIR}/include/*.h)
    coveralls_setup(
        "${coveralls_sources}"
        ON
        "${PROJECT_SOURCE_DIR}/tests/coveralls-cmake/cmake")
endif()
