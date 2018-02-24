cmake_minimum_required(VERSION 2.8)

add_library(shell-object OBJECT shell.c)

add_library(shell-static STATIC shell.c)

target_include_directories(shell-static PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions(shell-static PRIVATE Log_DISABLE_ALL)
