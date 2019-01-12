cmake_minimum_required(VERSION 3.13)
project(Siegenthaler_attack C)

set(CMAKE_C_STANDARD 11)

add_executable(Siegenthaler_attack main.c Generator.c Generator.h Attack.c Attack.h)