#include "metrics.hpp"

#include <cmath>

namespace huffman::metrics {

double shannon_entropy(const frequency::FrequencyTable& frequencies) {
    const std::uint64_t total = frequency::total_count(frequencies);
    if (total == 0) {
        return 0.0;
    }

    double entropy = 0.0;
    for (std::uint64_t count : frequencies) {
        if (count == 0) {
            continue;
        }
        const double p = static_cast<double>(count) / static_cast<double>(total);
        entropy -= p * std::log2(p);
    }
    return entropy;
}

double average_code_length(const frequency::FrequencyTable& frequencies, const code_table::CodeTable& codes) {
    const std::uint64_t total = frequency::total_count(frequencies);
    if (total == 0) {
        return 0.0;
    }

    double average = 0.0;
    for (std::size_t i = 0; i < frequencies.size(); ++i) {
        if (frequencies[i] == 0) {
            continue;
        }
        const double p = static_cast<double>(frequencies[i]) / static_cast<double>(total);
        average += p * static_cast<double>(codes[i].size());
    }
    return average;
}

double coding_efficiency(const frequency::FrequencyTable& frequencies, const code_table::CodeTable& codes) {
    const double average = average_code_length(frequencies, codes);
    if (average == 0.0) {
        return 0.0;
    }
    return shannon_entropy(frequencies) / average * 100.0;
}

}  // namespace huffman::metrics
