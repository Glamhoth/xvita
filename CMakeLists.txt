cmake_minimum_required(VERSION 2.8...3.16 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(MessageColors)
include(LoggingHelper)


if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    else()
        log_fatal("Please define VITASDK to point to your SDK path")
    endif()
endif()

project(xvita VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)

include(ClangFormat)
include(CTags)

add_library(common_project_options INTERFACE)
# include(CompilerWarnings)
target_compile_options(common_project_options INTERFACE "-Wl,-q -03 -std=gnu99")

set(XVITA_ELF_NAME "${PROJECT_NAME}.elf")
set(XVITA_ELF_PATH "${EXECUTABLE_OUTPUT_PATH}/${XVITA_ELF_NAME}")
set(XVITA_VELF_PATH "${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.velf")
set(XVITA_SKPRX_NAME "${PROJECT_NAME}.skprx")
set(XVITA_SKPRX_PATH "${EXECUTABLE_OUTPUT_PATH}/${XVITA_SKPRX_NAME}")

add_executable(${XVITA_ELF_NAME}
                src/main.c
                src/logger.c
    )
target_include_directories(${XVITA_ELF_NAME}
    PRIVATE     ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
target_link_libraries(${XVITA_ELF_NAME}
    PRIVATE     common_project_options
                gcc
                taihenForKernel_stub
                SceBtForDriver_stub
                SceIofilemgrForDriver_stub
                SceSysclibForDriver_stub
    )
set_target_properties(${XVITA_ELF_NAME}
    PROPERTIES LINK_FLAGS "-nostdlib"
    COMPILE_FLAGS "-D__VITA_KERNEL__"
    )

add_custom_target(${XVITA_SKPRX_NAME} ALL
    COMMAND vita-elf-create -e ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.yml ${XVITA_ELF_PATH} ${XVITA_VELF_PATH}
    COMMAND vita-make-fself -c ${XVITA_VELF_PATH} ${XVITA_SKPRX_PATH}
    )
add_dependencies(${XVITA_SKPRX_NAME} ${XVITA_ELF_NAME})
