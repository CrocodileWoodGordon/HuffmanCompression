#pragma once

#include "frequency.hpp"

#include <cstdint>
#include <filesystem>
#include <vector>

namespace huffman::archive_format {

struct Archive {
    std::uint64_t original_size = 0;
    frequency::FrequencyTable frequencies{};
    std::uint64_t bit_count = 0;
    std::vector<std::uint8_t> payload;
};

void write_archive(const std::filesystem::path& path, const Archive& archive);
Archive read_archive(const std::filesystem::path& path);

}  // namespace huffman::archive_format
