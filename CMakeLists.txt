cmake_minimum_required(VERSION 3.27)
project(LibrarySystem C)

set(CMAKE_C_STANDARD 11)

add_executable(LibrarySystem
        librarysys.c
        bookStruct.h
        studentStruct.h
        loansStruct.h
        displayHandler.h
        displayHandler.h
        fileHandler.c
        displayHandler.c
)
