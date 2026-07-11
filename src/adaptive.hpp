#pragma once

#include <filesystem>

namespace huffman::adaptive {

void compress_directory(const std::filesystem::path& directory);

}  // namespace huffman::adaptive
