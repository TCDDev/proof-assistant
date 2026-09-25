cmake_minimum_required(VERSION 3.20)

project(proof_assistant LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(
    proof
    INTERFACE
)

target_include_directories(
    proof
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(
    proof
    INTERFACE
        cxx_std_23
)

add_executable(
    proof_assistant
    proof_test.cpp
)

target_link_libraries(
    proof_assistant
    PRIVATE
        proof
)

target_compile_options(
    proof_assistant
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)


find_package(
    Python
    COMPONENTS
        Interpreter
        Development
    REQUIRED
)

find_package(
    pybind11
    CONFIG
    REQUIRED
)

pybind11_add_module(
    _proof
    src/bindings.cpp
)

target_link_libraries(
    _proof
    PRIVATE
        proof
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(
        proof_assistant
        PRIVATE
            -g3
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
    )

    target_link_options(
        proof_assistant
        PRIVATE
            -fsanitize=address,undefined
    )


endif()