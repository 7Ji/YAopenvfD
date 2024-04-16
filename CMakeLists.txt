cmake_minimum_required(VERSION 3.5)

project(YAopenvfD C)

if ("${VERSION}" STREQUAL "")
    execute_process(
        COMMAND bash scripts/version.sh
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE VERSION
        ERROR_QUIET)
    message("-- Version generated: ${VERSION}")
else()
    message("-- Version defined: ${VERSION}")
endif()

set_source_files_properties(src/version.c
    PROPERTIES COMPILE_DEFINITIONS VERSION=\"${VERSION}\")

add_executable(YAopenvfD
    src/collector/cpu.c
    src/collector/date.c
    src/collector/io.c
    src/collector/net.c
    src/collector/size.c
    src/collector/string.c
    src/collector/temp.c
    src/cli.c
    src/collector.c
    src/dots.c
    src/glyphs.c
    src/main.c
    src/openvfd.c
    src/reporter.c
    src/version.c
    src/watcher.c)

target_include_directories(YAopenvfD PRIVATE
    "include")

target_compile_options(YAopenvfD PRIVATE
    -Wall
    -Wextra)
