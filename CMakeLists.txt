#cmake_minimum_required(VERSION 3.18)

project(pyvm)

add_executable(
    pyvm main.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp
    object/pInteger.cpp
    object/pObject.cpp
    object/pString.cpp
    utils/bufferedInputStream.cpp
)
