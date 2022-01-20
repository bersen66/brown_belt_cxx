//
// Created by ortur on 20.01.2022.
//

#ifndef BROWN_BELT_NODE_BUILDER_H
#define BROWN_BELT_NODE_BUILDER_H

#include <deque>

struct Node {
    Node(int v, Node* p)
            : value(v)
            , parent(p)
    {}
    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent;
};

class NodeBuilder {
public:
    Node* CreateRoot(int value);

    Node* CreateLeftSon(Node* me, int value);

    Node* CreateRightSon(Node* me, int value);

private:
    std::deque<Node> nodes;
};

Node* Next(Node* me);

#endif //BROWN_BELT_NODE_BUILDER_H
