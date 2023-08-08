# dependencies that we have actual source code for

add_library(glarses_dependencies
    "./src/tlsf.c"          # two-level segregated fit memory allocator v3.1
            # StackWalker (windows-only, detailed stack inspection)
)

target_include_directories(glarses_dependencies PUBLIC
    "./include"
)