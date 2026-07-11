#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace huffman::frequency {

using FrequencyTable = std::array<std::uint64_t, 256>;

struct FrequencyEntry {
    std::uint16_t byte = 0;
    std::uint64_t count = 0;
    char display = '.';
};

FrequencyTable empty_table();
FrequencyTable count_bytes(const std::vector<std::uint8_t>& data);
void add_counts(FrequencyTable& target, const FrequencyTable& source);
std::uint64_t total_count(const FrequencyTable& frequencies);
std::vector<FrequencyEntry> sorted_entries(const FrequencyTable& frequencies);
std::string format_frequency_table(const FrequencyTable& frequencies);
char display_char(std::uint8_t byte);

}  // namespace huffman::frequency
