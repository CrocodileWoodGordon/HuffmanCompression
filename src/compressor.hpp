#pragma once

#include "frequency.hpp"

#include <filesystem>

namespace huffman::compressor {

void compress_file(const std::filesystem::path& input_path);
void compress_file_with_frequencies(
    const std::filesystem::path& input_path,
    const std::filesystem::path& archive_path,
    const std::filesystem::path& frequency_path,
    const std::filesystem::path& unzip_path,
    const frequency::FrequencyTable& tree_frequencies);

}  // namespace huffman::compressor
