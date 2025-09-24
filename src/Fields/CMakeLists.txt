cmake_minimum_required(VERSION 3.10)
project(Fields)
set(SOURCE_LIB FieldsCls.cpp FieldSolver.cpp)
add_library(Fields STATIC ${SOURCE_LIB})