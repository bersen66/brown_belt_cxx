//
// Created by ortur on 23.01.2022.
//

#ifndef BROWN_BELT_XML_H
#define BROWN_BELT_XML_H
#pragma once

#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

class Node {
public:
    Node(string name, unordered_map<string, string> attrs);

    const vector<Node>& Children() const;
    void AddChild(Node node);
    string_view Name() const;

    template <typename T>
    T AttributeValue(const string& name) const;

private:
    string name;
    vector<Node> children;
    unordered_map<string, string> attrs;
};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root;
};

Document Load(istream& input);


template <typename T>
inline T Node::AttributeValue(const string& name) const {
    istringstream attr_input(attrs.at(name));
    T result;
    attr_input >> result;
    return result;
}

#endif //BROWN_BELT_XML_H
