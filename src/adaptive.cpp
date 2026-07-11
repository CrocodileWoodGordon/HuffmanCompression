#include "adaptive.hpp"

#include "compressor.hpp"
#include "file_io.hpp"
#include "frequency.hpp"
#include "path_utils.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace huffman::adaptive {

void compress_directory(const std::filesystem::path& directory) {
    if (!std::filesystem::is_directory(directory)) {
        throw std::runtime_error("adaptive input is not a directory: " + directory.string());
    }

    frequency::FrequencyTable cumulative = frequency::empty_table();
    for (int stage = 1; stage <= 3; ++stage) {
        const path_utils::AdaptivePaths paths = path_utils::adaptive_paths(directory, stage);
        const std::vector<std::uint8_t> data = file_io::read_binary(paths.input);
        frequency::add_counts(cumulative, frequency::count_bytes(data));

        std::cout << "adaptive stage " << stage << ": " << paths.input << '\n';
        compressor::compress_file_with_frequencies(paths.input, paths.archive, paths.frequency, paths.unzip, cumulative);
    }
}

}  // namespace huffman::adaptive
