#include "path_utils.hpp"

#include <stdexcept>
#include <string>

namespace huffman::path_utils {

namespace {

std::filesystem::path with_stem_suffix(const std::filesystem::path& input, const std::string& suffix) {
    return input.parent_path() / (input.stem().string() + suffix);
}

}  // namespace

CompressionPaths compression_paths(const std::filesystem::path& input) {
    return {
        with_stem_suffix(input, ".huff"),
        with_stem_suffix(input, "_freq.txt"),
        with_stem_suffix(input, "_unzip.txt"),
    };
}

std::filesystem::path decompression_output_path(const std::filesystem::path& archive) {
    return with_stem_suffix(archive, "_unzip.txt");
}

AdaptivePaths adaptive_paths(const std::filesystem::path& directory, int stage) {
    if (stage < 1 || stage > 3) {
        throw std::runtime_error("adaptive stage must be 1, 2, or 3");
    }
    const std::string base = std::to_string(stage);
    return {
        directory / (base + ".txt"),
        directory / (base + "_adp.huff"),
        directory / (base + "_freq.txt"),
        directory / (base + "_unzip.txt"),
    };
}

}  // namespace huffman::path_utils
