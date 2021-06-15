#cmake_minimum_required(VERSION 3.18)

project(pyvm)

include_directories("./object" "./code" "./runtime" "utils" ".")

add_executable(
    pyvm 
    main.cpp
    code/binaryFileParser.cpp
    object/codeObject.cpp
    object/integerObject.cpp
    object/pObject.cpp
    object/stringObject.cpp
    object/klass.cpp
    object/functionObject.cpp
    utils/bufferedInputStream.cpp
    runtime/frame.cpp
    runtime/interpreter.cpp
    runtime/universe.cpp
)
