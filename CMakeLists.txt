cmake_minimum_required(VERSION 3.3)
project(SF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    calcSF.cpp
    myClass.h)

add_executable(SF ${SOURCE_FILES})