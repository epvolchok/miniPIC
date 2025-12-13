set(SOURCE_LIB FieldsCls.cpp FieldSolver.cpp)
add_library(Fields STATIC ${SOURCE_LIB})

target_include_directories(Fields
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)