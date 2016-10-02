cmake_minimum_required(VERSION 3.5)
project(putpixel)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

execute_process(
        COMMAND pkg-config --cflags --libs sdl2
        OUTPUT_VARIABLE SDL_CXX_FLAGS
        OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_CXX_FLAGS "--stdlib=libc++ -std=c++14 -lpthread ${CMAKE_CXX_FLAGS} ${SDL_CXX_FLAGS}")

set(SOURCE_FILES main.cpp util/tuple_reader.h util/vector_reader.h)

include_directories(../boost_1_61_0/)

add_executable(putpixel ${SOURCE_FILES})
