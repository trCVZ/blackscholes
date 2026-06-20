cmake_minimum_required(VERSION 3.29)
project(BlackScholesWASM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(blackscholes
    src/blackscholes.cpp
)

target_include_directories(blackscholes PRIVATE
    include
)

target_link_options(blackscholes PRIVATE
    "-O3"
    "--bind"
    "-sMODULARIZE=1"
    "-sEXPORT_ES6=1"
    "-sEXPORT_NAME=createBlackScholesModule"
    "-sENVIRONMENT=web"
    "-sALLOW_MEMORY_GROWTH=1"
)

set_target_properties(blackscholes PROPERTIES
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../front/src/wasm"
    OUTPUT_NAME "blackscholes"
)