#include "compressor.hpp"

#include "archive_format.hpp"
#include "bit_stream.hpp"
#include "code_table.hpp"
#include "decompressor.hpp"
#include "file_io.hpp"
#include "hash.hpp"
#include "huffman_tree.hpp"
#include "metrics.hpp"
#include "path_utils.hpp"

#include <iostream>
#include <stdexcept>

namespace huffman::compressor {

namespace {

void validate_codes_cover_input(const std::vector<std::uint8_t>& data, const code_table::CodeTable& codes) {
    for (std::uint8_t byte : data) {
        if (codes[byte].empty()) {
            throw std::runtime_error("Huffman tree frequencies do not cover input byte data");
        }
    }
}

void print_hash_check(const std::filesystem::path& input_path,
                      const std::filesystem::path& unzip_path,
                      const std::vector<std::uint8_t>& original,
                      const std::vector<std::uint8_t>& restored) {
    const hashing::HashPair original_hash = hashing::hash_bytes(original);
    const hashing::HashPair restored_hash = hashing::hash_bytes(restored);
    const bool ok = original_hash.first == restored_hash.first && original_hash.second == restored_hash.second;
    if (!ok) {
        throw std::runtime_error("hash verification failed for " + input_path.string());
    }

    std::cout << "verified " << input_path << " -> " << unzip_path
              << " hash1=" << hashing::to_hex(original_hash.first)
              << " hash2=" << hashing::to_hex(original_hash.second) << '\n';
}

}  // namespace

void compress_file_with_frequencies(
    const std::filesystem::path& input_path,
    const std::filesystem::path& archive_path,
    const std::filesystem::path& frequency_path,
    const std::filesystem::path& unzip_path,
    const frequency::FrequencyTable& tree_frequencies) {
    const std::vector<std::uint8_t> data = file_io::read_binary(input_path);
    const tree::NodePtr root = tree::build_tree(tree_frequencies);
    const code_table::CodeTable codes = code_table::build_code_table(root);
    validate_codes_cover_input(data, codes);

    const bit_stream::PackedBits packed = bit_stream::pack_bits(data, codes);
    archive_format::Archive archive;
    archive.original_size = static_cast<std::uint64_t>(data.size());
    archive.frequencies = tree_frequencies;
    archive.bit_count = packed.bit_count;
    archive.payload = packed.bytes;

    archive_format::write_archive(archive_path, archive);
    file_io::write_text(frequency_path, frequency::format_frequency_table(tree_frequencies));
    decompressor::decompress_archive_to(archive_path, unzip_path);

    const std::vector<std::uint8_t> restored = file_io::read_binary(unzip_path);
    print_hash_check(input_path, unzip_path, data, restored);

    const double entropy = metrics::shannon_entropy(tree_frequencies);
    const double average_length = metrics::average_code_length(tree_frequencies, codes);
    const double efficiency = metrics::coding_efficiency(tree_frequencies, codes);
    std::cout << "wrote " << archive_path << " entropy=" << entropy
              << " avg_len=" << average_length << " efficiency=" << efficiency << "%\n";
}

void compress_file(const std::filesystem::path& input_path) {
    const std::vector<std::uint8_t> data = file_io::read_binary(input_path);
    const frequency::FrequencyTable frequencies = frequency::count_bytes(data);
    const path_utils::CompressionPaths paths = path_utils::compression_paths(input_path);
    compress_file_with_frequencies(input_path, paths.archive, paths.frequency, paths.unzip, frequencies);
}

}  // namespace huffman::compressor
