cmake_minimum_required(VERSION 2.8.4)

project(fs_readsync)

set(SOURCE_FILES main.c)

include_directories("/usr/local/include" "${PROJECT_SOURCE_DIR}/deps")

link_directories("/usr/local/lib")

add_executable(fs_readsync ${SOURCE_FILES})

target_link_libraries(fs_readsync "libuv.a")
