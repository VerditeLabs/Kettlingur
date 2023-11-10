cmake_minimum_required(VERSION 3.27)
project(Kettlingur C)

set(CMAKE_C_STANDARD 23)

include_directories(.)

add_executable(Kettlingur
        kettlingur.c
        kettlingur.h
        thirdparty/elf.h
        disasm.c
        shorthand.h
        ee.c
        iop.c
        vu.c)

target_compile_options(Kettlingur PUBLIC -fwrapv -g3 -Weverything -fPIC -fPIE -static -static-pie)
target_compile_options(Kettlingur PUBLIC -Weverything -Wno-padded -Wno-vla -Wno-reserved-id-macro -Wno-documentation-unknown-command -Wno-poison-system-directories -Wno-format-nonliteral -Wno-unsafe-buffer-usage -Wno-gnu-folding-constant)
target_compile_options(Kettlingur PUBLIC -Wno-missing-prototypes -Wno-unused-parameter -Wno-unused-function)