#pragma once

#include "code_table.hpp"

#include <cstdint>
#include <vector>

namespace huffman::bit_stream {

struct PackedBits {
    std::vector<std::uint8_t> bytes;
    std::uint64_t bit_count = 0;
};

class BitReader {
public:
    BitReader(const std::vector<std::uint8_t>& bytes, std::uint64_t bit_count);

    bool has_next() const;
    bool next();

private:
    const std::vector<std::uint8_t>& bytes_;
    std::uint64_t bit_count_ = 0;
    std::uint64_t position_ = 0;
};

PackedBits pack_bits(const std::vector<std::uint8_t>& data, const code_table::CodeTable& codes);

}  // namespace huffman::bit_stream
