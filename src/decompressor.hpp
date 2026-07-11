#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace huffman::decompressor {

std::vector<std::uint8_t> decompress_to_memory(const std::filesystem::path& archive_path);
void decompress_archive_to(const std::filesystem::path& archive_path, const std::filesystem::path& output_path);
void decompress_file(const std::filesystem::path& archive_path);

}  // namespace huffman::decompressor
