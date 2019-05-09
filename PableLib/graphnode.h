#pragma once
#include <vector>

using GraphNodeValue = int;

struct GraphNode
{
    int id;
    GraphNodeValue value;

    GraphNode(): id(-1), value(0) {}
    GraphNode(int id, const GraphNodeValue &value): id(id), value(value) {}
};

using NodeList = std::vector<GraphNode>;
using EdgeList = std::vector<std::vector<int>>;
