cmake_minimum_required(VERSION 2.8.4)

project(hello)

set(SOURCE_FILES main.c)

include_directories("/usr/local/include")

link_directories("/usr/local/lib")

add_executable(hello ${SOURCE_FILES})

target_link_libraries(hello "libuv.a")