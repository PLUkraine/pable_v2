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

class Graph
{
private:
    static int MAX_NODES;
    std::vector<GraphNode> mNodes;
    std::vector<std::vector<int>> mForw;
    std::vector<std::vector<int>> mBack;

public:
    Graph();

    void setValue(int id, const GraphNodeValue &newValue);
    GraphNodeValue value(int id) const;

private:
    void recompute();
};
