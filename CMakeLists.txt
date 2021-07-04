cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_COMPILER /usr/bin/g++)

project(pvm)

include_directories("./object" "./code" "./runtime" "./utils" "./memory")

add_executable(
    pvm 
    main.cpp
    code/binaryFileParser.cpp
    object/codeObject.cpp
    object/integerObject.cpp
    object/object.cpp
    object/stringObject.cpp
    object/klass.cpp
    object/functionObject.cpp
    object/listObject.cpp
    object/dictObject.cpp
    object/cellObject.cpp
    object/typeObject.cpp
    utils/bufferedInputStream.cpp
    runtime/frame.cpp
    runtime/interpreter.cpp
    runtime/universe.cpp
    memory/heap.cpp
    memory/space.cpp
    memory/oopClosure.cpp
)
