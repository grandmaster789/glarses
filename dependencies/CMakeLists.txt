# dependencies that we have actual source code for

add_library(glarses_dependencies
            src/dummy.cpp
)

target_include_directories(glarses_dependencies PUBLIC
    "./include"
)