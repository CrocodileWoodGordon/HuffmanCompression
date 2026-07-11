cmake_minimum_required(VERSION 3.16)

project(HuffmanCompression LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Huffman
    src/main.cpp
    src/cli.cpp
    src/path_utils.cpp
    src/file_io.cpp
    src/frequency.cpp
    src/huffman_tree.cpp
    src/code_table.cpp
    src/bit_stream.cpp
    src/archive_format.cpp
    src/compressor.cpp
    src/decompressor.cpp
    src/adaptive.cpp
    src/hash.cpp
    src/metrics.cpp
)

target_include_directories(Huffman PRIVATE src)

