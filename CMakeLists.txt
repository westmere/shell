cmake_minimum_required(VERSION 2.8)

option(USE_STATIC_SHELL_LIBRARY "Build json_jsmn static library." OFF)
option(USE_OBJECT_SHELL_LIBRARY "Build json_jsmn object library." ON)

if(USE_OBJECT_SHELL_LIBRARY)
    add_library(shell-object OBJECT shell.c)
endif(USE_OBJECT_SHELL_LIBRARY)

if(USE_STATIC_SHELL_LIBRARY)
    add_library(shell-static STATIC shell.c)

    target_include_directories(shell-static PUBLIC ${CMAKE_CURRENT_LIST_DIR})
    target_compile_definitions(shell-static PRIVATE Log_DISABLE_ALL)
endif(USE_STATIC_SHELL_LIBRARY)
