#cmake_minimum_required(VERSION 3.18)

project(pvm)

include_directories("./object" "./code" "./runtime" "./utils" ".")

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
    utils/bufferedInputStream.cpp
    runtime/frame.cpp
    runtime/interpreter.cpp
    runtime/universe.cpp
)
