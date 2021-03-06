cmake_minimum_required(VERSION 3.2.3)
project(libcppffi CXX)

set(COVERALLS OFF CACHE BOOL "Turn on coveralls")
set(WERROR ON CACHE BOOL "Treat warnings as errors")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/tests/coveralls-cmake/cmake")

message(STATUS "Coveralls: ${COVERALLS}")
if(COVERALLS)
    include(Coveralls)
    coveralls_turn_on_coverage()
endif()

include(CheckCXXCompilerFlag)

macro(add_compiler_flags)
    foreach(flag ${ARGV})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endforeach()
endmacro()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if(WERROR)
        add_compiler_flags(-Werror)
    endif()

    add_compiler_flags(-pedantic-errors)
    add_compiler_flags(-fvisibility=hidden)
    add_compiler_flags(-fstrict-aliasing)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compiler_flags(-Wall)
    add_compiler_flags(-Wextra)
    add_compiler_flags(-fdiagnostics-show-option)
    add_compiler_flags(-Wconversion)
    add_compiler_flags(-Wold-style-cast)
    add_compiler_flags(-Wfloat-equal)
    add_compiler_flags(-Wlogical-op)
    add_compiler_flags(-Wundef)
    add_compiler_flags(-Wredundant-decls)
    add_compiler_flags(-Wshadow)
    add_compiler_flags(-Wstrict-overflow=5)
    add_compiler_flags(-Wwrite-strings)
    add_compiler_flags(-Wpointer-arith)
    add_compiler_flags(-Wcast-qual)
    add_compiler_flags(-Wformat=2)
    add_compiler_flags(-Wswitch-default)
    add_compiler_flags(-Wmissing-include-dirs)
    add_compiler_flags(-Wcast-align)
    add_compiler_flags(-Wswitch-enum)
    add_compiler_flags(-Wnon-virtual-dtor)
    add_compiler_flags(-Wctor-dtor-privacy)
    add_compiler_flags(-Wsign-conversion)
    add_compiler_flags(-Wdisabled-optimization)
    add_compiler_flags(-Weffc++)
    add_compiler_flags(-Winline)
    add_compiler_flags(-Winvalid-pch)
    add_compiler_flags(-Wmissing-declarations)
    add_compiler_flags(-Woverloaded-virtual)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        add_compiler_flags(-pedantic)
    else()
        add_compiler_flags(-Wpedantic)
        add_compiler_flags(-Wno-missing-field-initializers)
    endif()

    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
        add_compiler_flags(-Wnoexcept)
    endif()

    if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        add_compiler_flags(-Wdouble-promotion)
        add_compiler_flags(-Wtrampolines)
        add_compiler_flags(-Wzero-as-null-pointer-constant)
        add_compiler_flags(-Wuseless-cast)
        add_compiler_flags(-Wvector-operation-performance)
        add_compiler_flags(-Wsized-deallocation)
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 6.0 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0)
        add_compiler_flags(-Wshift-overflow=2)
        add_compiler_flags(-Wnull-dereference)
        add_compiler_flags(-Wduplicated-cond)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compiler_flags(-Weverything)
    add_compiler_flags(-Wpedantic)
    add_compiler_flags(-Wno-c++98-compat)
    add_compiler_flags(-Wno-c++98-compat-pedantic)
    add_compiler_flags(-Wno-weak-vtables)
    add_compiler_flags(-Wno-padded)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compiler_flags(/std:c++latest) # for post c++14 updates in MSVC
    add_compiler_flags(/WX)
    add_compiler_flags(/W4) # /Wall is too aggressive - even the standard C headers give thousands of errors...
endif()

find_path(FFI_INCLUDE_PATH ffi.h PATHS ${FFI_INCLUDE_DIR})
if(EXISTS "${FFI_INCLUDE_PATH}/ffi.h")
    set(FFI_HEADER ffi.h CACHE INTERNAL "")
    set(HAVE_FFI_H 1 CACHE INTERNAL "")
else()
    find_path(FFI_INCLUDE_PATH ffi/ffi.h PATHS ${FFI_INCLUDE_DIR})
    if(EXISTS "${FFI_INCLUDE_PATH}/ffi/ffi.h")
        set(FFI_HEADER ffi/ffi.h CACHE INTERNAL "")
        set(HAVE_FFI_FFI_H CACHE INTERNAL "")
    endif()
endif()

if(NOT FFI_HEADER)
    message(FATAL_ERROR "libffi includes couldn't be found")
endif()

find_library(FFI_LIBRARY_PATH ffi PATHS ${FFI_LIBRARY_DIR})
if(NOT FFI_LIBRARY_PATH)
    message(FATAL_ERROR "libffi couldn't be found")
endif()

set(CPPFFI_INCLUDE_FOLDER "${PROJECT_SOURCE_DIR}/include/cppffi")
set(CPPFFI_PARTS_FOLDER "${CPPFFI_INCLUDE_FOLDER}/parts")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${CPPFFI_INCLUDE_FOLDER})
include_directories(SYSTEM ${FFI_INCLUDE_PATH})
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/tests/doctest/doctest")

enable_testing()
add_subdirectory(tests)
