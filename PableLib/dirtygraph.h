#ifndef DIRTYGRAPH_H
#define DIRTYGRAPH_H

#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <optional>

#define UNUSED(x) (void)(x)

class DirtyGraph
{
private:
    int mCount;
    std::vector<std::set<int>> mForward;
    std::vector<std::set<int>> mReverse;
    std::vector<std::optional<int>> mValues;

public:
    DirtyGraph(int nodes);

    void addEdge(int from, int to);
    void removeEdge(int from, int to);

    std::optional<int> reevaluate(int where);

    bool setValue(int v, int value);
    void setInvalid(int v);
    std::optional<int> getValue(int where);
    std::vector<std::optional<int>> getValues(const std::vector<int> &where);
    void clear();
    int countDependencies(int where);

    std::vector<int> detectForwardCycle(int from);
    std::vector<int> detectReverseCycle(int from);

private:
    static const char WHITE;
    static const char GRAY;
    static const char BLACK;

    std::vector<int> findCycle(int v, std::vector<char> &colors, const std::vector<std::set<int>>& edges);
    void updateDependentOn(const std::vector<int> &vertices);
    void updateDependentOn(const std::vector<int> &vertices, std::vector<char> &color);
    std::optional<int> updateDirectValue(int where);

    bool handleCycleDetection(int where);

    void errorUpdateDfs(int v,
                        const std::vector<std::set<int> > &edges,
                        std::vector<char>& color);
    bool topologySort(int v,
                      const std::vector<std::set<int> > &edges,
                      std::vector<char>& color,
                      std::vector<int> &result);
};

#endif // DIRTYGRAPH_H
