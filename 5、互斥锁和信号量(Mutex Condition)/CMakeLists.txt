cmake_minimum_required(VERSION 3.9)
project(Reactor)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        MutexTest
        MutexTest.cpp
        AtomicIntegerT.h
        Mutex.h
        Condition.h
        CountDownLatch.cpp
        CountDownLatch.h
        CurrentThread.h
        Thread.cpp
        Thread.h
        Timestamp.cpp
        Timestamp.h
)