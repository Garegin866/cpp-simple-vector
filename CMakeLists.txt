cmake_minimum_required(VERSION 3.31)
project(cpp_simple_vector)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_simple_vector main.cpp simple_vector.h tests.h array_ptr.h)
