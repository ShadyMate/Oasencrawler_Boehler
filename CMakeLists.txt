cmake_minimum_required(VERSION 3.27)
project(Oasencrawler_Boehler)

set(CMAKE_CXX_STANDARD 14)

include_directories(Headers)
include_directories(Headers/include)

add_executable(Oasencrawler_Boehler
        Headers/include/catch_amalgamated.hpp
        Headers/include/enemy.h
        Headers/include/item.h
        Headers/include/player.h
        Headers/include/world.h
        source/src/maintest.cpp
        source/src/player.cpp
        source/src/world.cpp
        source/main.cpp
        source/catch_amalgamated.cpp)
