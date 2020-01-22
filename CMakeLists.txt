cmake_minimum_required(VERSION 3.7)

project(ex09 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "mpicxx")
find_package(Boost COMPONENTS mpi REQUIRED)

add_executable(ex09 main.cpp)
target_link_libraries(ex09 PUBLIC Boost::mpi)
