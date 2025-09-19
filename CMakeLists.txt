cmake_minimum_required(VERSION 3.20)

project(extended_exception_examples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

#set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
#add_subdirectory(slides-examples)

#add_executable(extended_exception extended_exception.cpp)
#target_link_libraries(extended_exception PRIVATE stdc++exp)
#add_executable(simple_cxa_throw simple_cxa_throw.cpp)
#add_executable(simple_cxa_throw_hook simple_cxa_throw_hook.cpp)
#add_executable(throw1 throw1.cpp)
#target_link_libraries(throw1 PRIVATE stdc++exp)
#add_executable(throw2 throw2.cpp)
#target_link_libraries(throw2 PRIVATE stdc++exp)


# Add the source files for your shared library
#add_library(throw_stacktrace SHARED lib_throw_stacktrace.cpp)

# Ensure position-independent code (PIC) is enabled for the shared library
# This is often default for shared libraries on Linux, but explicitly setting it is good practice.
#set_target_properties(throw_stacktrace PROPERTIES POSITION_INDEPENDENT_CODE ON)

#add_executable(test_lib1 test_lib1.cpp)
#target_link_libraries(test_lib1 PRIVATE stdc++exp throw_stacktrace)
