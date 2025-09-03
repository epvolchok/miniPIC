cmake_minimum_required(VERSION 3.10)
project(Fields_slover LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER=g++-14)

set(SOURCE_EXE main.cpp)

include_directories(./Fields)
include_directories(./Diagnostics)

add_executable(Fields_slover ${SOURCE_EXE})

add_subdirectory(./Fields)
add_subdirectory(./Diagnostics)

target_link_libraries(Fields_slover Fields Diag)


