#include "graphloopdetector.h"
#include "utils.h"

using IdEdge = std::pair<int, int>;


GraphLoopDetector::GraphLoopDetector(int maxNodes)
    : mColor(maxNodes, 0)
{
}

bool GraphLoopDetector::hasCycle(const EdgeList &forwardList, int startId)
{
    assert(forwardList.size() <= mColor.size());
    clearVector(mColor);
    return dfs(forwardList, startId);
}

bool GraphLoopDetector::dfs(const EdgeList &g, int v)
{
    mColor[v] = 1;
    for (const int to : g[v])
    {
        if (mColor[to] == 0)
        {
            if (dfs(g, to))
                return true;
        }
        if (mColor[to] == 1)
            return true;
    }
    mColor[v] = 2;
    return false;
}
