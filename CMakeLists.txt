cmake_minimum_required(VERSION 2.8)
project(waitkey)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfatal-errors -pedantic -std=c99 -Wl,--no-undefined -fPIC -Wall -Wextra -Wconversion -Werror=return-type -Werror=implicit-function-declaration")

include_directories("./")

add_executable(example1 "examples/example1.c")
add_executable(example2 "examples/example2.c")
