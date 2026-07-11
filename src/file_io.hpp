#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace huffman::file_io {

std::vector<std::uint8_t> read_binary(const std::filesystem::path& path);
void write_binary(const std::filesystem::path& path, const std::vector<std::uint8_t>& data);
void write_text(const std::filesystem::path& path, const std::string& text);

}  // namespace huffman::file_io
