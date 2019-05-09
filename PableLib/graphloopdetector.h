#pragma once
#include "graphnode.h"

class GraphLoopDetector
{
private:
    std::vector<int> mColor;

public:
    GraphLoopDetector(int maxNodes);
    bool hasCycle(const EdgeList &forwardList, int startId);

private:
    bool dfs(const EdgeList &g, int v);
};
