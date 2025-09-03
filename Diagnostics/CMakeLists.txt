cmake_minimum_required(VERSION 3.10)
project(Diag)
set(SOURCE_LIB Diagnostics.cpp)
add_library(Diag STATIC ${SOURCE_LIB})