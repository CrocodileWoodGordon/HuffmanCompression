#include "archive_format.hpp"

#include <array>
#include <fstream>
#include <stdexcept>

namespace huffman::archive_format {

namespace {

constexpr std::array<char, 8> kMagic = {'H', 'U', 'F', 'F', '1', '7', '\r', '\n'};

void write_u64(std::ostream& out, std::uint64_t value) {
    for (int i = 0; i < 8; ++i) {
        out.put(static_cast<char>((value >> (i * 8)) & 0xFFU));
    }
}

std::uint64_t read_u64(std::istream& in) {
    std::uint64_t value = 0;
    for (int i = 0; i < 8; ++i) {
        const int ch = in.get();
        if (ch == EOF) {
            throw std::runtime_error("unexpected end of archive");
        }
        value |= (static_cast<std::uint64_t>(static_cast<unsigned char>(ch)) << (i * 8));
    }
    return value;
}

}  // namespace

void write_archive(const std::filesystem::path& path, const Archive& archive) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("failed to open archive for writing: " + path.string());
    }

    out.write(kMagic.data(), static_cast<std::streamsize>(kMagic.size()));
    write_u64(out, archive.original_size);
    write_u64(out, archive.bit_count);
    for (std::uint64_t count : archive.frequencies) {
        write_u64(out, count);
    }
    if (!archive.payload.empty()) {
        out.write(reinterpret_cast<const char*>(archive.payload.data()), static_cast<std::streamsize>(archive.payload.size()));
    }
    if (!out) {
        throw std::runtime_error("failed to write archive: " + path.string());
    }
}

Archive read_archive(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("failed to open archive for reading: " + path.string());
    }

    std::array<char, kMagic.size()> magic{};
    in.read(magic.data(), static_cast<std::streamsize>(magic.size()));
    if (!in || magic != kMagic) {
        throw std::runtime_error("invalid .huff archive header: " + path.string());
    }

    Archive archive;
    archive.original_size = read_u64(in);
    archive.bit_count = read_u64(in);
    for (std::uint64_t& count : archive.frequencies) {
        count = read_u64(in);
    }

    archive.payload.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    if (archive.bit_count > archive.payload.size() * 8ULL) {
        throw std::runtime_error("archive bit count exceeds payload size: " + path.string());
    }
    return archive;
}

}  // namespace huffman::archive_format
