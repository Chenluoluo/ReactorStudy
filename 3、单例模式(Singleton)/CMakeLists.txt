cmake_minimum_required(VERSION 3.9)
project(Reactor)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        SingletonTest
        SingletonTest.cpp
        Singleton.h
)