cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MyExecutable
    main.cpp
    helpers.cpp
    nodes.cpp
    package.cpp
    storage_types.cpp
    tests.cpp
    helpers.hxx
    nodes.hxx
    package.hxx
    storage_types.hxx
    types.hxx
)