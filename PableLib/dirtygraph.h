#ifndef DIRTYGRAPH_H
#define DIRTYGRAPH_H

#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <optional>

#define UNUSED(x) (void)(x)

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
    bool setValue(int v, int value);
    void setInvalid(int v);
    std::optional<int> getValue(int where);
    std::vector<std::optional<int>> getValues(std::vector<int> where);
    void clear();
    int countDependencies(int where);

    std::vector<int> detectForwardCycle(int from);
    std::vector<int> detectReverseCycle(int from);

private:
    static const char WHITE;
    static const char GRAY;
    static const char BLACK;

    std::vector<int> findCycle(int v, const std::vector<std::vector<int>>& edges);
    void updateDependentOn(std::vector<int> vertices);
    std::optional<int> updateDirectValue(int where);

    void errorUpdateDfs(int v, std::vector<std::vector<int> > &edges, std::vector<char>& color);
    bool topologySort(int v, std::vector<std::vector<int> > &edges, std::vector<char>& color, std::vector<int> &result);
};

#endif // DIRTYGRAPH_H
