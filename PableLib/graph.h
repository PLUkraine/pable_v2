#pragma once
#include "graphnode.h"
#include "graphloopdetector.h"

class Graph
{
private:
    static int MAX_NODES;
    GraphLoopDetector mLoopDetector;
    std::vector<GraphNode> mNodes;
    std::vector<std::vector<int>> mForw;
    std::vector<std::vector<int>> mBack;

public:
    Graph();

    bool setValue(int id, const GraphNodeValue &newValue);
    GraphNodeValue value(int id) const;

private:
    bool validate(int idChanged);
    void recompute(int idChanged);

    void initializeGraph();
};
