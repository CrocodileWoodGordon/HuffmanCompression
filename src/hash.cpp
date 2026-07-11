#include "hash.hpp"

#include <iomanip>
#include <sstream>

namespace huffman::hashing {

namespace {

constexpr std::uint64_t kSeed1 = 0x243f6a8885a308d3ULL;
constexpr std::uint64_t kSeed2 = 0x13198a2e03707344ULL;

}  // namespace

std::uint64_t splitmix64(std::uint64_t value) {
    value += 0x9e3779b97f4a7c15ULL;
    value = (value ^ (value >> 30U)) * 0xbf58476d1ce4e5b9ULL;
    value = (value ^ (value >> 27U)) * 0x94d049bb133111ebULL;
    return value ^ (value >> 31U);
}

std::uint64_t universal_hash_bytes(const std::vector<std::uint8_t>& data) {
    std::uint64_t h1 = kSeed1;
    std::uint64_t h2 = kSeed2;
    for (std::uint8_t byte : data) {
        h1 = splitmix64(h1 ^ byte);
        h2 = splitmix64(h2 + static_cast<std::uint64_t>(byte) * 1315423911ULL);
    }
    return h1 ^ (h2 << 1U);
}

std::uint64_t rolling_mix_hash_bytes(const std::vector<std::uint8_t>& data) {
    std::uint64_t hash = 1469598103934665603ULL;
    std::uint64_t index = 1;
    for (std::uint8_t byte : data) {
        hash ^= splitmix64(static_cast<std::uint64_t>(byte) + index * 0x9e3779b97f4a7c15ULL);
        hash *= 1099511628211ULL;
        ++index;
    }
    return splitmix64(hash ^ static_cast<std::uint64_t>(data.size()));
}

HashPair hash_bytes(const std::vector<std::uint8_t>& data) {
    return {universal_hash_bytes(data), rolling_mix_hash_bytes(data)};
}

std::string to_hex(std::uint64_t value) {
    std::ostringstream out;
    out << std::hex << std::setfill('0') << std::setw(16) << value;
    return out.str();
}

}  // namespace huffman::hashing
