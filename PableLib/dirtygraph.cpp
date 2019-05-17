#include "dirtygraph.h"

const char DirtyGraph::WHITE = 0;
const char DirtyGraph::GRAY = 1;
const char DirtyGraph::BLACK = 2;

DirtyGraph::DirtyGraph(int nodes)
    : mCount(nodes),
      mForward(mCount, std::vector<int>()),
      mReverse(mCount, std::vector<int>()),
      mValues(mCount, 0)
{

}

void DirtyGraph::addEdge(int from, int to)
{
    // add forward and backward edge
    mForward[from].push_back(to);
    mReverse[to].push_back(from);

    // detect forward cycle
    auto cycle = detectForwardCycle(from);
    if (cycle.empty())
    {
        // all is good, update this and all dependent nodes
        updateDirectValue(from);
        updateDependentOn({from});
    }
    else
    {
        // nullopt all values dependent on cycle
        mValues[cycle[0]] = std::nullopt;
        updateDependentOn(cycle);
    }
}

bool DirtyGraph::setValue(int v, int value)
{
    // do nothing if error
    if (!mValues[v].has_value())
    {
        return false;
    }

    // set value and update dependent nodes
    mValues[v] = value;
    updateDependentOn({v});
    return mValues[v].has_value();
}

void DirtyGraph::setInvalid(int v)
{
    // invalidate all dependent nodes
    mValues[v] = std::nullopt;
    updateDependentOn({v});
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

void DirtyGraph::updateDependentOn(std::vector<int> nodes)
{
    // do not visit same nodes twice
    std::vector<char> color(mCount, WHITE);
    // update invalid first.
    for (int v : nodes)
    {
        if (color[v] == WHITE && !mValues[v].has_value())
        {
            errorUpdateDfs(v, mReverse, color);
        }
    }
    // here every invalid node is reached. update all remaining nodes
    for (int v : nodes)
    {
        if (color[v] == WHITE)
        {
            // should have value
            assert(mValues[v].has_value());

            std::vector<int> order;
            bool canSort = topologySort(v, mReverse, color, order);
            std::reverse(order.begin(), order.end());
            // should be able to sort
            assert(canSort);
            for (int to : order)
            {
                updateDirectValue(to);
            }
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

void DirtyGraph::errorUpdateDfs(int v, std::vector<std::vector<int>> &edges, std::vector<char> &color)
{
    color[v] = BLACK;
    mValues[v] = std::nullopt;
    for (int to : edges[v])
    {
        if (color[to] == WHITE)
            errorUpdateDfs(to, edges, color);
    }
}

bool DirtyGraph::topologySort(int v, std::vector<std::vector<int>> &edges, std::vector<char> &color, std::vector<int>& result)
{
    color[v] = GRAY;
    for (int to : edges[v])
    {
        if (color[to] == WHITE)
        {
            if (!topologySort(to, edges, color, result))
            {
                return false;
            }
        }
        if (color[to] == GRAY)
        {
            result.clear();
            return false;
        }
    }
    result.push_back(v);
    color[v] = BLACK;
    return true;
}
