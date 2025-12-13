set(SOURCE_LIB Diagnostics.cpp)
add_library(Diag STATIC ${SOURCE_LIB})

target_link_libraries(Diag
    PUBLIC
        Fields
)