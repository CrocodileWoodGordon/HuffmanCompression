#include "adaptive.hpp"
#include "cli.hpp"
#include "compressor.hpp"
#include "decompressor.hpp"

#include <exception>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        const huffman::cli::Command command = huffman::cli::parse(argc, argv);
        switch (command.mode) {
            case huffman::cli::Mode::Help:
                std::cout << huffman::cli::help_text();
                return 0;
            case huffman::cli::Mode::Compress:
                huffman::compressor::compress_file(command.path);
                return 0;
            case huffman::cli::Mode::Decompress:
                huffman::decompressor::decompress_file(command.path);
                return 0;
            case huffman::cli::Mode::Adaptive:
                huffman::adaptive::compress_directory(command.path);
                return 0;
        }
    } catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }

    std::cerr << "error: unreachable command mode\n";
    return 1;
}
