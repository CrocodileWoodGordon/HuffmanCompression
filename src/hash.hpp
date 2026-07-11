#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace huffman::hashing {

struct HashPair {
    std::uint64_t first = 0;
    std::uint64_t second = 0;
};

std::uint64_t splitmix64(std::uint64_t value);
std::uint64_t universal_hash_bytes(const std::vector<std::uint8_t>& data);
std::uint64_t rolling_mix_hash_bytes(const std::vector<std::uint8_t>& data);
HashPair hash_bytes(const std::vector<std::uint8_t>& data);
std::string to_hex(std::uint64_t value);

}  // namespace huffman::hashing
