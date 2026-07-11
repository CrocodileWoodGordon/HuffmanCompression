set_project("HuffmanCompression")
set_languages("c++17")

target("Huffman")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("src")

