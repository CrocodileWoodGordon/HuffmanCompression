#include "decompressor.hpp"

#include "archive_format.hpp"
#include "bit_stream.hpp"
#include "file_io.hpp"
#include "huffman_tree.hpp"
#include "path_utils.hpp"

#include <stdexcept>

namespace huffman::decompressor {

namespace {

std::vector<std::uint8_t> decode_archive(const archive_format::Archive& archive) {
    std::vector<std::uint8_t> output;
    output.reserve(static_cast<std::size_t>(archive.original_size));

    if (archive.original_size == 0) {
        return output;
    }

    const tree::NodePtr root = tree::build_tree(archive.frequencies);
    if (!root) {
        throw std::runtime_error("archive has data but no Huffman tree");
    }

    if (root->is_leaf()) {
        output.assign(static_cast<std::size_t>(archive.original_size), static_cast<std::uint8_t>(root->symbol));
        return output;
    }

    bit_stream::BitReader reader(archive.payload, archive.bit_count);
    tree::NodePtr current = root;
    while (output.size() < archive.original_size) {
        if (!reader.has_next()) {
            throw std::runtime_error("archive ended before enough bytes were decoded");
        }

        current = reader.next() ? current->right : current->left;
        if (!current) {
            throw std::runtime_error("archive bit stream does not match Huffman tree");
        }

        if (current->is_leaf()) {
            output.push_back(static_cast<std::uint8_t>(current->symbol));
            current = root;
        }
    }

    return output;
}

}  // namespace

std::vector<std::uint8_t> decompress_to_memory(const std::filesystem::path& archive_path) {
    return decode_archive(archive_format::read_archive(archive_path));
}

void decompress_archive_to(const std::filesystem::path& archive_path, const std::filesystem::path& output_path) {
    file_io::write_binary(output_path, decompress_to_memory(archive_path));
}

void decompress_file(const std::filesystem::path& archive_path) {
    decompress_archive_to(archive_path, path_utils::decompression_output_path(archive_path));
}

}  // namespace huffman::decompressor
