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

    auto cycle = detectForwardCycle(from);
    if (cycle.empty())
    {
        // recompute this value
        // recompute backward values
    }
    else
    {
        // nullopt all values dependent on cycle
        mValues[cycle[0]] = std::nullopt;
        updateNodesDependantOn(cycle);
    }
}

bool DirtyGraph::setValue(int v, int value)
{
    // do nothing if error
    if (!mValues[v].has_value())
    {
        return false;
    }

    mValues[v] = value;
    updateNodesDependantOn({v});
    return mValues[v].has_value();
}

void DirtyGraph::setInvalid(int v)
{
    mValues[v] = std::nullopt;
    updateNodesDependantOn({v});
}

std::optional<int> DirtyGraph::getValue(int where)
{
    return mValues[where];
}

std::vector<std::optional<int>> DirtyGraph::getValues(std::vector<int> where)
{
    std::vector<std::optional<int>> answer(where.size());
    std::transform(where.begin(), where.end(), answer.begin(), [this](int v) {
        return mValues[v];
    });
    return answer;
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
    const char WHITE = 0;
    const char GRAY = 1;
    const char BLACK = 2;
    std::vector<char> colors(mCount, WHITE);

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

void DirtyGraph::updateNodesDependantOn(std::vector<int> nodes)
{
    std::vector<char> used(mCount, false);
    // update invalid first
    for (int v : nodes)
    {
        if (!mValues[v].has_value())
        {
            errorUpdateDfs(v, used);
        }
    }
    // then update normal ones
    for (int v : nodes)
    {
        if (mValues[v].has_value())
        {
            // update normally
        }
    }
}

std::optional<int> DirtyGraph::updateDirectValue(int where)
{
    int answer = 0;
    for (int depend : mForward[where])
    {
        auto val = mValues[depend];
        if (!val.has_value()) {
            mValues[where] = std::nullopt;
            return mValues[where];
        }
        answer += val.value_or(0);
    }

    mValues[where] = answer;
    return mValues[where];
}

void DirtyGraph::errorUpdateDfs(int v, std::vector<char> &used)
{
    used[v] = true;
    mValues[v] = std::nullopt;
    for (int to : mReverse[v])
    {
        if (!used[to])
            errorUpdateDfs(to, used);
    }
}
