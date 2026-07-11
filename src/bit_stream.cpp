#include "bit_stream.hpp"

#include <stdexcept>

namespace huffman::bit_stream {

BitReader::BitReader(const std::vector<std::uint8_t>& bytes, std::uint64_t bit_count)
    : bytes_(bytes), bit_count_(bit_count) {}

bool BitReader::has_next() const {
    return position_ < bit_count_;
}

bool BitReader::next() {
    if (!has_next()) {
        throw std::runtime_error("attempted to read past end of bit stream");
    }
    const std::uint64_t byte_index = position_ / 8;
    const std::uint64_t bit_index = 7 - (position_ % 8);
    ++position_;
    return ((bytes_[static_cast<std::size_t>(byte_index)] >> bit_index) & 1U) != 0;
}

PackedBits pack_bits(const std::vector<std::uint8_t>& data, const code_table::CodeTable& codes) {
    PackedBits packed;

    std::uint8_t current = 0;
    int used = 0;
    for (std::uint8_t byte : data) {
        const auto& code = codes[byte];
        if (code.empty()) {
            throw std::runtime_error("missing Huffman code for byte");
        }
        for (bool bit : code) {
            current = static_cast<std::uint8_t>((current << 1U) | (bit ? 1U : 0U));
            ++used;
            ++packed.bit_count;
            if (used == 8) {
                packed.bytes.push_back(current);
                current = 0;
                used = 0;
            }
        }
    }

    if (used != 0) {
        current = static_cast<std::uint8_t>(current << (8 - used));
        packed.bytes.push_back(current);
    }

    return packed;
}

}  // namespace huffman::bit_stream
