//
// Created by ortur on 20.01.2022.
//

#include <cassert>
#include "node_builder.h"

Node *NodeBuilder::CreateRoot(int value) {
    nodes.emplace_back(value, nullptr);
    return &nodes.back();
}

Node *NodeBuilder::CreateLeftSon(Node *me, int value) {
    assert( me->left == nullptr );
    nodes.emplace_back(value, me);
    me->left = &nodes.back();
    return me->left;
}

Node *NodeBuilder::CreateRightSon(Node *me, int value) {
    assert( me->right == nullptr );
    nodes.emplace_back(value, me);
    me->right = &nodes.back();
    return me->right;
}




Node* Next(Node* me) {

    // Всего одна вершина
    if (me->left == nullptr && me->right == nullptr && me->parent == nullptr)
        return nullptr;

    if (me -> left == nullptr && me->right == nullptr) {

        Node* result = me->parent;
        while (result && result->value < me->value) {
            result = result->parent;
        }
        return result;
    }

    if (me -> right) {

        Node* result = me -> right;
        while (result -> left) {
            result = result->left;
        }
        return result;

    }


}
