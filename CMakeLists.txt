cmake_minimum_required(VERSION 3.20)

project(
    KlyroDB
    VERSION 0.1.0
    DESCRIPTION "A high-performance embedded relational database engine"
    LANGUAGES CXX
)

# KlyroDB Options and strict compilation flags
include(cmake/KlyroDBOptions.cmake)

# Ensure C++20 is used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add subdirectories
add_subdirectory(src)

if(KLYRO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(KLYRO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(KLYRO_BUILD_PYTHON)
    include(FetchContent)
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.10.4
    )
    FetchContent_MakeAvailable(pybind11)
    
    pybind11_add_module(_klyro src/python/module.cpp)
    target_link_libraries(_klyro PRIVATE klyro)
endif()
