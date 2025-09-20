cmake_minimum_required(VERSION 3.10)
project(dumblang)

# https://en.cppreference.com/w/c/compiler_support/23
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB Sources src/**/*.c src/*.c)
add_executable(dumblang ${Sources})
