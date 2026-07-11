#include "cli.hpp"

#include <stdexcept>

namespace huffman::cli {

Command parse(int argc, char* argv[]) {
    if (argc == 1) {
        return {Mode::Help, {}};
    }

    if (argc != 3) {
        throw std::runtime_error("invalid argument count\n" + help_text());
    }

    const std::string option = argv[1];
    const std::filesystem::path path = argv[2];
    if (option == "-c") {
        return {Mode::Compress, path};
    }
    if (option == "-x") {
        return {Mode::Decompress, path};
    }
    if (option == "-a") {
        return {Mode::Adaptive, path};
    }

    throw std::runtime_error("unknown option: " + option + "\n" + help_text());
}

std::string help_text() {
    return "Usage:\n"
           "  Huffman -c <file>   Compress a file\n"
           "  Huffman -x <file>   Decompress a .huff file\n"
           "  Huffman -a <dir>    Adaptive compression for 1.txt, 2.txt, 3.txt\n";
}

}  // namespace huffman::cli
