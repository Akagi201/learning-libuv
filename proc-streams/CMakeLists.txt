cmake_minimum_required(VERSION 2.8.4)

project(proc-streams)

set(SOURCE_FILES main.c)

include_directories("/usr/local/include")

link_directories("/usr/local/lib")

add_executable(proc-streams ${SOURCE_FILES})

target_link_libraries(proc-streams "libuv.a")