cmake_minimum_required(VERSION 3.29)
project(bmp_steg C)

set(CMAKE_C_STANDARD 11)

add_executable(bmp_steg main.c
        cbmp/cbmp.c
        cbmp/cbmp.h)

