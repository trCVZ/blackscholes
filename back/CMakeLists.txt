cmake_minimum_required(VERSION 3.15)
project(BlackScholesWASM LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/blackscholes.cpp
    src/bindings.cpp
)

add_executable(blackscholes ${SOURCES})

target_include_directories(blackscholes PRIVATE include)

# --- Emscripten / WebAssembly flags ---
target_link_options(blackscholes PRIVATE
    "SHELL:-s WASM=1"
    "SHELL:-s MODULARIZE=1"
    "SHELL:-s EXPORT_ES6=1"
    "SHELL:-s USE_ES6_IMPORT_META=1"
    "SHELL:-s ALLOW_MEMORY_GROWTH=1"
    "SHELL:--bind"
)

# Output name
set_target_properties(blackscholes PROPERTIES
    OUTPUT_NAME "blackscholes"
)

# Copy vers frontend (IMPORTANT: adapter chemin réel)
add_custom_command(TARGET blackscholes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_SOURCE_DIR}/../front/src/lib/wasm

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/blackscholes.js
        ${CMAKE_SOURCE_DIR}/../front/src/lib/wasm/blackscholes.js

    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/blackscholes.wasm
        ${CMAKE_SOURCE_DIR}/../front/src/lib/wasm/blackscholes.wasm

    COMMENT "Copy WASM to frontend"
)