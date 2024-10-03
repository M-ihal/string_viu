cmake_minimum_required(VERSION 3.17.0)
project(string_viu_tests)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(string_viu_tests string_viu_tests.cpp string_viu.h)