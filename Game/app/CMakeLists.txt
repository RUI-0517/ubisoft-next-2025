cmake_minimum_required(VERSION 3.20.0)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PCH_HEADER "${CMAKE_SOURCE_DIR}/stdafx.h")

add_subdirectory("math")