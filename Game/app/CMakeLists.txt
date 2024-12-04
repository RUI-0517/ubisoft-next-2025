cmake_minimum_required(VERSION 3.20.0)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

option(USE_SSE4_1 "Compile with SSE4.1 support" OFF)

if(USE_SSE4_1)
  add_compile_options(-msse4.1)
endif()

set(PCH_HEADER "${CMAKE_SOURCE_DIR}/stdafx.h")

add_subdirectory("math")