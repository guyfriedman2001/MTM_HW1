cmake_minimum_required(VERSION 3.29)
project(mtmmergehw1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(mtmmergehw1
    Block.cpp
    Block.h
    BlockChain.cpp
    BlockChain.h
    Transaction.cpp
    Transaction.h
    Utilities.cpp
    Utilities.h)
