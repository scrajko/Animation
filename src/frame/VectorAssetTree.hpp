
#ifndef VECTOR_ASSET_TREE_HPP
#define VECTOR_ASSET_TREE_HPP

/**
 * Asset Trees are currently still just theoretical ideas for the project
 * This is not implemented.
 */

// move transformation stuff somewhere else
struct Transformation {
    Coordinate origin; // should this be here?
    Coordinate translation;
    double     rotation; // degrees
    bool       mirror;
};

struct VectorAssetNode {

    Transformation transformation;
    std::vector<VectorAssetNode*> children;
};

class VectorAssetTree {
    VectorAssetTree(const std::string& filename);
    VectorObject traverse() const;
    VectorAssetNode* root;
};

// Perhaps a "Frame" should just be a special type of VectorObject root?
// Main program then has a list of VectorObjects called frames.
// Note: for this function, VectorObject is 1-dimensional:
/**
 * The point of this function is to
 * traverse the tree to generate a simple list of
 * points and connections
 */
VectorObject VectorAssetTree::traverse() const
{
    VectorObject asset;

    // Traverse tree

    // If node is leaf:
    append node points to asset's points.
    append nodes connections to asset's connections.

    // Once done traversing tree:
    return asset;
}


#endif // VECTOR_ASSET_TREE_HPP
