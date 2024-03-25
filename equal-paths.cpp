#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
// Helper function to traverse the tree. It returns a pair of values:
// - the minimum depth of a leaf node found in the subtree rooted at 'node'
// - the maximum depth of a leaf node found in the subtree rooted at 'node'
// If the subtree is empty, it returns {INT_MAX, INT_MIN} to signify no paths.
std::pair<int, int> checkEqualPathsHelper(Node* node, int depth) {
    if (!node) {
        return {std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};
    }
    if (!node->left && !node->right) {
        // Leaf node, return its depth
        return {depth, depth};
    }

    auto left = checkEqualPathsHelper(node->left, depth + 1);
    auto right = checkEqualPathsHelper(node->right, depth + 1);

    // Combine the results from the left and right subtrees
    int minDepth = std::min(left.first, right.first);
    int maxDepth = std::max(left.second, right.second);

    return {minDepth, maxDepth};
}

bool equalPaths(Node* root) {
    if (root == nullptr) {
        return true; // An empty tree has all paths of equal length.
    }

    std::pair<int, int> depths = checkEqualPathsHelper(root, 0);
    int minDepth = depths.first;
    int maxDepth = depths.second;
    return minDepth == maxDepth;
}
