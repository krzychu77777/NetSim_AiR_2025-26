cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# UWAGA: linijki 11-20 pożyczyłem z internetu - ogarniają GoogleTesta
#                                                   ~Krzychu
include(FetchContent)

# Download GoogleTest for unit tests
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
# On MSVC this prevents overriding runtime flags; harmless elsewhere
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MyExecutable
    main.cpp
    factory.cpp
    helpers.cpp
    nodes.cpp
    package.cpp
    storage_types.cpp
    tests.cpp
    factory.hxx
    helpers.hxx
    nodes.hxx
    package.hxx
    storage_types.hxx
    types.hxx
)

# UWAGA: jeszcze kilka czareodziejskich linijek do GoogleTesta:
#                                                   ~Krzychu
# include project headers and link tests with GoogleTest
target_include_directories(MyExecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# Link to GTest without its provided main since project defines its own test main in main.cpp
target_link_libraries(MyExecutable PRIVATE GTest::gtest)

enable_testing()
include(GoogleTest)
gtest_discover_tests(MyExecutable)