#pragma once

#include "frequency.hpp"

#include <cstdint>
#include <memory>

namespace huffman::tree {

struct Node {
    std::uint64_t weight = 0;
    int symbol = -1;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    bool is_leaf() const {
        return !left && !right;
    }
};

using NodePtr = std::shared_ptr<Node>;

NodePtr build_tree(const frequency::FrequencyTable& frequencies);

}  // namespace huffman::tree
