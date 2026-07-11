#include "code_table.hpp"

namespace huffman::code_table {

namespace {

void fill_codes(const tree::NodePtr& node, Code& current, CodeTable& table) {
    if (!node) {
        return;
    }

    if (node->is_leaf()) {
        if (current.empty()) {
            current.push_back(false);
        }
        table[static_cast<std::size_t>(node->symbol)] = current;
        return;
    }

    current.push_back(false);
    fill_codes(node->left, current, table);
    current.pop_back();

    current.push_back(true);
    fill_codes(node->right, current, table);
    current.pop_back();
}

}  // namespace

CodeTable build_code_table(const tree::NodePtr& root) {
    CodeTable table;
    Code current;
    fill_codes(root, current, table);
    return table;
}

}  // namespace huffman::code_table
