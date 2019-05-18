#include "dirtygraph.h"

const char DirtyGraph::WHITE = 0;
const char DirtyGraph::GRAY = 1;
const char DirtyGraph::BLACK = 2;

DirtyGraph::DirtyGraph(int nodes)
    : mCount(nodes),
      mForward(mCount),
      mReverse(mCount),
      mValues(mCount, 0)
{

}

void DirtyGraph::addEdge(int from, int to)
{
    // add forward and backward edge
    mForward[from].insert(to);
    mReverse[to].insert(from);

    handleCycleDetection(from);
}
void DirtyGraph::removeEdge(int from, int to)
{
    mForward[from].erase(to);
    mReverse[to].erase(from);
}

std::optional<int> DirtyGraph::reevaluate(int where)
{
    if (!handleCycleDetection(where))
    {
        assert(mValues[where] == std::nullopt);
        return std::nullopt;
    }

    std::vector<char> color(mCount, WHITE);
    std::vector<int> order;
    bool canSort = topologySort(where, mForward, color, order);
    assert(canSort);
    for (int to : order)
    {
        updateDirectValue(to);
    }

    updateDependentOn({where});
    return mValues[where];
}

void DirtyGraph::reevaluateAll()
{
    std::vector<char> color(mCount, WHITE);
    for (int v=0; v<mCount; ++v)
    {
        if (color[v] == WHITE) {
            std::vector<int> order;
            bool canSort = topologySort(v, mForward, color, order);
            if (!canSort) {
                mValues[v] = std::nullopt;
            }
            else {
                for (int to : order) {
                    updateDirectValue(to);
                }
                updateDependentOn({v});
            }
        }
        else if (color[v] == GRAY) {
            mValues[v] = std::nullopt;
        }
    }
}

//void DirtyGraph::addEdges(const std::vector<std::pair<int, int> > &edges)
//{
//    for (const auto& edge : edges)
//    {
//        mForward[edge.first].push_back(edge.second);
//        mReverse[edge.second].push_back(edge.first);
//    }

//    std::vector<int> cycleVertices;
//    std::vector<char> color(mCount, WHITE);
//    for (const auto& edge : edges)
//    {
//        auto cycle = findCycle(edge.first, color, mForward);
//        cycleVertices.insert(cycleVertices.end(), cycle.begin(), cycle.end());
//    }

//    // TODO
//}

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

std::vector<std::optional<int>> DirtyGraph::getValues(const std::vector<int> &where)
{
    std::vector<std::optional<int>> answer(where.size());
    std::transform(where.begin(), where.end(), answer.begin(), [this](int v) {
        return mValues[v];
    });
    return answer;
}

void DirtyGraph::clear()
{
    mForward.assign(mCount, std::set<int>());
    mReverse.assign(mCount, std::set<int>());
    mValues.assign(mCount, 0);
}

int DirtyGraph::countDependencies(int where)
{
    return mForward[where].size();
}

std::vector<int> DirtyGraph::detectForwardCycle(int from)
{
    std::vector<char> colors(mCount, WHITE);
    return findCycle(from, colors, mForward);
}

std::vector<int> DirtyGraph::detectReverseCycle(int from)
{
    std::vector<char> colors(mCount, WHITE);
    return findCycle(from, colors, mReverse);
}

std::vector<int> DirtyGraph::findCycle(int v, std::vector<char> &colors, const std::vector<std::set<int> > &edges)
{
    std::vector<int> par(mCount, -1);
    int cycleEnd = -1;
    int cycleSt = -1;
    std::function<bool(int)> dfs = [&](int v)
    {
        colors[v] = GRAY;
        for (int to : edges[v])
        {
            if (colors[to] == GRAY) {
                colors[to] = BLACK;
                cycleEnd = v;
                cycleSt = to;
                return true;
            }
            if (colors[to] == WHITE) {
                par[to] = v;
                if (dfs(to)) {
                    colors[v] = BLACK;
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

void DirtyGraph::updateDependentOn(const std::vector<int> &vertices)
{
    // do not visit same nodes twice
    std::vector<char> color(mCount, WHITE);
    updateDependentOn(vertices, color);
}

void DirtyGraph::updateDependentOn(const std::vector<int> &nodes, std::vector<char> &color)
{
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
                // do not recompute for currentNode
                if (to == v) continue;
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

bool DirtyGraph::handleCycleDetection(int where)
{
    auto cycle = detectForwardCycle(where);
    if (!cycle.empty())
    {
        // nullopt all values dependent on cycle
        mValues[cycle[0]] = std::nullopt;
        updateDependentOn({cycle[0]});
        return false;
    }
    return true;
}

void DirtyGraph::errorUpdateDfs(int v,
                                const std::vector<std::set<int> > &edges,
                                std::vector<char> &color)
{
    color[v] = BLACK;
    mValues[v] = std::nullopt;
    for (int to : edges[v])
    {
        if (color[to] == WHITE)
            errorUpdateDfs(to, edges, color);
    }
}

bool DirtyGraph::topologySort(int v,
                              const std::vector<std::set<int> > &edges,
                              std::vector<char> &color,
                              std::vector<int>& result)
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
