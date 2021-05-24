#cmake_minimum_required(VERSION 3.18)

project(pyvm)

add_executable(
    pyvm 
    main.cpp
    code/binaryFileParser.cpp
    code/codeObject.cpp
    object/integerObject.cpp
    object/pObject.cpp
    object/stringObject.cpp
    object/klass.cpp
    object/functionObject.cpp
    utils/bufferedInputStream.cpp
    runtime/frameObject.cpp
    runtime/interpreter.cpp
    runtime/universe.cpp
)
