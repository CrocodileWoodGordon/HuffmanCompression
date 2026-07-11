#include "huffman_tree.hpp"

#include <algorithm>
#include <queue>
#include <vector>

namespace huffman::tree {

namespace {

struct QueueItem {
    std::uint64_t weight = 0;
    int min_symbol = 0;
    std::uint64_t order = 0;
    NodePtr node;
};

struct CompareQueueItem {
    bool operator()(const QueueItem& lhs, const QueueItem& rhs) const {
        if (lhs.weight != rhs.weight) {
            return lhs.weight > rhs.weight;
        }
        if (lhs.min_symbol != rhs.min_symbol) {
            return lhs.min_symbol > rhs.min_symbol;
        }
        return lhs.order > rhs.order;
    }
};

}  // namespace

NodePtr build_tree(const frequency::FrequencyTable& frequencies) {
    std::priority_queue<QueueItem, std::vector<QueueItem>, CompareQueueItem> queue;
    std::uint64_t order = 0;

    for (int i = 0; i < static_cast<int>(frequencies.size()); ++i) {
        if (frequencies[static_cast<std::size_t>(i)] == 0) {
            continue;
        }
        auto node = std::make_shared<Node>();
        node->weight = frequencies[static_cast<std::size_t>(i)];
        node->symbol = i;
        queue.push({node->weight, i, order++, node});
    }

    if (queue.empty()) {
        return nullptr;
    }

    while (queue.size() > 1) {
        QueueItem first = queue.top();
        queue.pop();
        QueueItem second = queue.top();
        queue.pop();

        auto parent = std::make_shared<Node>();
        parent->weight = first.weight + second.weight;
        parent->symbol = -1;
        parent->left = first.node;
        parent->right = second.node;

        queue.push({parent->weight, std::min(first.min_symbol, second.min_symbol), order++, parent});
    }

    return queue.top().node;
}

}  // namespace huffman::tree
