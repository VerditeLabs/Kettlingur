cmake_minimum_required(VERSION 3.27)
project(Kettlingur C)

set(CMAKE_C_STANDARD 23)

include_directories(.)

add_executable(Kettlingur
        kettlingur.c
        kettlingur.h
        thirdparty/elf.h)

target_compile_options(Kettlingur PUBLIC -fwrapv -g3 -Og -fPIC -fPIE -static -static-pie)