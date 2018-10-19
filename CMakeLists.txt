cmake_minimum_required(VERSION 3.6)
project(nvtensor CXX)


set(CXX_STANDARD "17" CACHE STRING "Use c++ standard")
set(CXX_STD "cxx_std_${CXX_STANDARD}")

add_library(nvtensor INTERFACE)
target_compile_features(nvtensor INTERFACE "${CXX_STD}")


target_compile_definitions(nvtensor INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:
        _SCL_SECURE_NO_WARNINGS
        _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING>)


target_include_directories(nvtensor INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(DIRECTORY include/nvtensor DESTINATION include)

enable_testing()
add_subdirectory(tests)
