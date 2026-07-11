#pragma once

#include "huffman_tree.hpp"

#include <array>
#include <cstdint>
#include <vector>

namespace huffman::code_table {

using Code = std::vector<bool>;
using CodeTable = std::array<Code, 256>;

CodeTable build_code_table(const tree::NodePtr& root);

}  // namespace huffman::code_table
