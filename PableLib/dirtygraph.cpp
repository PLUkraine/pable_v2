#include "dirtygraph.h"

DirtyGraph::DirtyGraph(int nodes)
    : mCount(nodes),
      mForward(mCount, std::vector<int>()),
      mReverse(mCount, std::vector<int>()),
      mValues(mCount, 0)
{

}

void DirtyGraph::addEdge(int from, int to)
{
    mForward[from].push_back(to);
    mReverse[to].push_back(from);

    // recursion check
}

void DirtyGraph::setValue(int v, int value)
{
    int newValue = value;
    for (int to : mForward[v])
    {
        newValue += mValues[to].value_or(0);
    }
    mValues[v] = newValue;

    // update dependencies
}

void DirtyGraph::clear()
{
    mForward.assign(mCount, std::vector<int>());
    mReverse.assign(mCount, std::vector<int>());
    mValues.assign(mCount, 0);
}

int DirtyGraph::countDependencies(int where)
{
    return mForward[where].size();
}

std::vector<int> DirtyGraph::detectForwardCycle(int from)
{
    return findCycle(from, mForward);
}

std::vector<int> DirtyGraph::detectReverseCycle(int from)
{
    return findCycle(from, mReverse);
}

std::vector<int> DirtyGraph::findCycle(int v, const std::vector<std::vector<int> > &edges)
{
    const int WHITE = 0;
    const int GRAY = 1;
    const int BLACK = 2;
    std::vector<int> colors(mCount, WHITE);

    std::vector<int> par(mCount, -1);
    int cycleEnd = -1;
    int cycleSt = -1;
    std::function<bool(int)> dfs = [&](int v)
    {
        colors[v] = GRAY;
        for (int to : edges[v])
        {
            if (colors[to] == GRAY) {
                cycleEnd = v;
                cycleSt = to;
                return true;
            }
            if (colors[to] == WHITE) {
                par[to] = v;
                if (dfs(to)) {
                    return true;
                }
            }
        }
        colors[v] = BLACK;
        return false;
    };

    dfs(v);

    if (cycleEnd != -1)
    {
        std::vector<int> cycle;

        while (cycleSt != cycleEnd)
        {
            cycle.push_back(cycleEnd);
            cycleEnd = par[cycleEnd];
        }
        cycle.push_back(cycleEnd);

        return cycle;
    }
    return {};
}
