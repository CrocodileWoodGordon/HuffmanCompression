#include "frequency.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace huffman::frequency {

FrequencyTable empty_table() {
    FrequencyTable table{};
    table.fill(0);
    return table;
}

FrequencyTable count_bytes(const std::vector<std::uint8_t>& data) {
    FrequencyTable table = empty_table();
    for (std::uint8_t byte : data) {
        ++table[byte];
    }
    return table;
}

void add_counts(FrequencyTable& target, const FrequencyTable& source) {
    for (std::size_t i = 0; i < target.size(); ++i) {
        target[i] += source[i];
    }
}

std::uint64_t total_count(const FrequencyTable& frequencies) {
    std::uint64_t total = 0;
    for (std::uint64_t count : frequencies) {
        total += count;
    }
    return total;
}

char display_char(std::uint8_t byte) {
    if (byte >= 33 && byte <= 126) {
        return static_cast<char>(byte);
    }
    return '.';
}

std::vector<FrequencyEntry> sorted_entries(const FrequencyTable& frequencies) {
    std::vector<FrequencyEntry> entries;
    entries.reserve(256);
    for (std::size_t i = 0; i < frequencies.size(); ++i) {
        if (frequencies[i] == 0) {
            continue;
        }
        entries.push_back({static_cast<std::uint16_t>(i), frequencies[i], display_char(static_cast<std::uint8_t>(i))});
    }

    std::sort(entries.begin(), entries.end(), [](const FrequencyEntry& lhs, const FrequencyEntry& rhs) {
        if (lhs.count != rhs.count) {
            return lhs.count > rhs.count;
        }
        return lhs.byte < rhs.byte;
    });
    return entries;
}

std::string format_frequency_table(const FrequencyTable& frequencies) {
    std::ostringstream out;
    for (const FrequencyEntry& entry : sorted_entries(frequencies)) {
        out << std::setw(3) << entry.byte << "   " << entry.display << "   " << entry.count << '\n';
    }
    return out.str();
}

}  // namespace huffman::frequency
