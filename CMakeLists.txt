cmake_minimum_required(VERSION 3.8)
project(Preprocessor)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/Source/Preprocessor.cpp src/Header/Preprocessor.h src/Source/lab3.cpp src/Source/Util.cpp src/Header/Util.h src/Source/Node.cpp src/Header/Node.h)

add_executable(Preprocessor ${SOURCE_FILES})