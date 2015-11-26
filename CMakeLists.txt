cmake_minimum_required(VERSION 3.2)
project(http)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(./)
aux_source_directory(./minix MINIX)
add_executable(minix ${MINIX})
