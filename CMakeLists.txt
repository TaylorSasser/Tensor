cmake_minimum_required(VERSION 3.6)
project(nvtensor CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB META ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/*.hpp)
file(GLOB BLAS ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/*.hpp)
file(GLOB TENSOR ${CMAKE_CURRENT_SOURCE_DIR}/include/meta/*.hpp)

add_library(nvtensor INTERFACE)
target_compile_features(nvtensor INTERFACE)
target_include_directories(nvtensor INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

