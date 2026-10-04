cmake_minimum_required(VERSION 3.14)

project(NetSim LANGUAGES CXX)

include(FetchContent)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

# Prevent GoogleTest from overriding the parent project's runtime library on MSVC.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(netsim
    src/factory.cpp
    src/helpers.cpp
    src/nodes.cpp
    src/package.cpp
    src/storage_types.cpp
)

target_include_directories(netsim
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(netsim PUBLIC cxx_std_17)
set_target_properties(netsim PROPERTIES CXX_EXTENSIONS OFF)

add_executable(netsim_tests
    tests/main.cpp
    tests/tests.cpp
)

target_link_libraries(netsim_tests
    PRIVATE
        netsim
        GTest::gtest
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(netsim_tests)
