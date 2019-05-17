#ifndef DIRTYGRAPH_H
#define DIRTYGRAPH_H

#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <optional>

class DirtyGraph
{
private:
    int mCount;
    std::vector<std::vector<int>> mForward;
    std::vector<std::vector<int>> mReverse;
    std::vector<std::optional<int>> mValues;

public:
    DirtyGraph(int nodes);

    void addEdge(int from, int to);
    void setValue(int v, int value);
    void clear();
    int countDependencies(int where);

    std::vector<int> detectForwardCycle(int from);
    std::vector<int> detectReverseCycle(int from);

private:
    std::vector<int> findCycle(int v, const std::vector<std::vector<int>>& edges);
};

#endif // DIRTYGRAPH_H
