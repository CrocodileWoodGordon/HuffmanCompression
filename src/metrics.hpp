#pragma once

#include "code_table.hpp"
#include "frequency.hpp"

namespace huffman::metrics {

double shannon_entropy(const frequency::FrequencyTable& frequencies);
double average_code_length(const frequency::FrequencyTable& frequencies, const code_table::CodeTable& codes);
double coding_efficiency(const frequency::FrequencyTable& frequencies, const code_table::CodeTable& codes);

}  // namespace huffman::metrics
