cmake_minimum_required(VERSION 2.8.12)

project(bashGen C)

set(SOURCES
    list.c
    main.c
    util.c
)

add_executable(generator ${SOURCES})
