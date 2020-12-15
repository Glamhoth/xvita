option(OPTIONS_WARNINGS_AS_ERRORS
       "Treat compiler warnings as errors"
       TRUE)

set(XVITA_WARNINGS -Wall
                   -Wextra
                   -Wpedantic
                   -Wshadow
                   -Wcast-align
                   -Wunused
                   -Wnull-dereference
                   -Wdouble-promotion
                   -Wformat=2
                   -Wmisleading-indentation
                   -Wduplicated-cond
                   -Wduplicated-branches
                   -Wlogical-op
   )

if(WARNINGS_AS_ERRORS)
    set(XVITA_WARNINGS ${XVITA_WARNINGS} -Werror)
endif()

target_compile_options(common_project_options INTERFACE ${XVITA_WARNINGS})
