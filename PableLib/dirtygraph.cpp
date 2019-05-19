#include "dirtygraph.h"
#include <numeric>

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
    auto cycleVertex = topologySort(where, mForward, color, order);
    assert(!cycleVertex.has_value());
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
    std::vector<int> allVertices(mCount);
    std::iota(allVertices.begin(), allVertices.end(), 0);
    reevaluateIn(allVertices, color);
}

void DirtyGraph::reevaluateIn(const std::vector<int> &vertices, std::vector<char> color)
{
    for (int v : vertices)
    {
        if (color[v] == WHITE) {
            std::vector<int> order;
            auto cycleVertex = topologySort(v, mForward, color, order);
            if (cycleVertex.has_value()) {
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
            auto cycleVertex = topologySort(v, mReverse, color, order);
            std::reverse(order.begin(), order.end());
            // should be able to sort
            assert(!cycleVertex.has_value());
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
    std::vector<char> mColor(mCount, WHITE);
    std::vector<int> _;
    auto cycleVertex = topologySort(where, mForward, mColor, _);
    if (cycleVertex.has_value())
    {
        // nullopt all values dependent on cycle
        mValues[*cycleVertex] = std::nullopt;
        updateDependentOn({*cycleVertex});
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

std::optional<int> DirtyGraph::topologySort(int v,
                              const std::vector<std::set<int> > &edges,
                              std::vector<char> &color,
                              std::vector<int>& result)
{
    color[v] = GRAY;
    for (int to : edges[v])
    {
        if (color[to] == WHITE)
        {
            auto cycleVertex = topologySort(to, edges, color, result);
            if (cycleVertex.has_value())
            {
                return cycleVertex;
            }
        }
        if (color[to] == GRAY)
        {
            result.clear();
            return to;
        }
    }
    result.push_back(v);
    color[v] = BLACK;
    return std::nullopt;
}
