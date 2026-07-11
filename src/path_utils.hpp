#pragma once

#include <filesystem>

namespace huffman::path_utils {

struct CompressionPaths {
    std::filesystem::path archive;
    std::filesystem::path frequency;
    std::filesystem::path unzip;
};

struct AdaptivePaths {
    std::filesystem::path input;
    std::filesystem::path archive;
    std::filesystem::path frequency;
    std::filesystem::path unzip;
};

CompressionPaths compression_paths(const std::filesystem::path& input);
std::filesystem::path decompression_output_path(const std::filesystem::path& archive);
AdaptivePaths adaptive_paths(const std::filesystem::path& directory, int stage);

}  // namespace huffman::path_utils
