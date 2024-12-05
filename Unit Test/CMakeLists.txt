cmake_minimum_required(VERSION 3.20.0)
project(test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Determine architecture directory
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "")
endif()

add_subdirectory(Math)