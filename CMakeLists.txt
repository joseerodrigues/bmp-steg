cmake_minimum_required(VERSION 3.29)
project(bmp_steg C)

set(CMAKE_C_STANDARD 11)

add_executable(bmp_steg_enc main_enc.c
        qdbmp/qdbmp.c)

add_executable(bmp_steg_dec main_dec.c
        qdbmp/qdbmp.c)

