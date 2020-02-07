cmake_minimum_required(VERSION 3.16)

project(ex09 LANGUAGES CXX)

# set(CMAKE_CXX_COMPILER "mpicxx")
# find_package(Boost COMPONENTS mpi REQUIRED)
find_package(MPI REQUIRED)

add_executable(ex09 main.cpp)
# target_link_libraries(ex09 PUBLIC Boost::mpi MPI::MPI_CXX)
target_link_libraries(ex09 PUBLIC MPI::MPI_CXX)
