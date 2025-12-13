cmake_minimum_required(VERSION 3.10)
project(Fields_slover LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Fields_solver main.cpp)

add_subdirectory(Fields)
add_subdirectory(Diagnostics)

target_link_libraries(Fields_solver
    PRIVATE
        Fields
        Diag)


