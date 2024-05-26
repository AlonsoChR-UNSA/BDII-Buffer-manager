cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp
        Classes/Block.h
        Classes/Page.h
        Classes/Frame.h
        Classes/BufferPool.cpp
        Classes/BufferPool.h
        Classes/BufferManager.h
)
