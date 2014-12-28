cmake_minimum_required(VERSION 2.8.4)

project(idle)

set(SOURCE_FILES main.c)

include_directories("/usr/local/include" "${PROJECT_SOURCE_DIR}/deps")

link_directories("/usr/local/lib")

add_executable(idle ${SOURCE_FILES})

target_link_libraries(idle "libuv.a")
