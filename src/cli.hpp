#pragma once

#include <filesystem>
#include <string>

namespace huffman::cli {

enum class Mode {
    Help,
    Compress,
    Decompress,
    Adaptive,
};

struct Command {
    Mode mode = Mode::Help;
    std::filesystem::path path;
};

Command parse(int argc, char* argv[]);
std::string help_text();

}  // namespace huffman::cli
