cmake_minimum_required(VERSION 3.6)
project(nvtensor-tests CXX)


file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(nvtensor-tests ${TEST_SOURCES})