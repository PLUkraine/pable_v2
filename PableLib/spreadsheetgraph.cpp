#include "spreadsheetgraph.h"

SpreadsheetGraph::SpreadsheetGraph()
{

}

void SpreadsheetGraph::setExpressionWithoutUpdate(const CellIndex &at, const Expression &expression)
{
    auto it = mExpr.find(at);
    if (it != mExpr.end())
        purgeCellDependencies(at);

    mExpr[at] = expression;
    createCellDepencencies(at);
}

void SpreadsheetGraph::updateExpression(const CellIndex &atIndex, const Expression &expression)
{
    setExpressionWithoutUpdate(atIndex, expression);
    updateAll();
}

void SpreadsheetGraph::updateAll()
{
    auto isSetSingleton = [](const std::unordered_set<CellIndex> &comp) {
        return comp.size() == 1;
    };

    GraphCondensation condensation;
    auto strongComponents = condensation.condensate(mAllVertices, mForward, mReverse);
    auto compEnd = std::remove_if(strongComponents.begin(), strongComponents.end(), isSetSingleton);

    // no cycles
    if (compEnd == strongComponents.begin())
    {
        // get global sort order and reevaluate ENTIRE graph
        auto order = condensation.topologicalSort(mAllVertices, mForward);
        for (auto it = order.rbegin(); it != order.rend(); ++it)
        {
            mExpr.at(*it).evaluate(*this);
        }
    }
    // there are cycles!
    else
    {
        // propagate error using reverse graph
        UsedMap used;
        for (auto it = strongComponents.begin(); it != compEnd; ++it)
        {
            auto anyCycleVertex = *it->begin();
            propagateErrorDfs(anyCycleVertex, used);
        }
    }
}

const Expression &SpreadsheetGraph::getExpression(const CellIndex &at) const
{
    static Expression nullExpression;
    auto it = mExpr.find(at);
    return it == mExpr.end() ? nullExpression : it->second;
}

std::optional<int> SpreadsheetGraph::getValue(const CellIndex &at) const
{
    auto it = mExpr.find(at);
    if (it == mExpr.end()) {
        return 0;
    }

    const Expression &expr = it->second;
    if (!expr.wasEvaluated())
        return std::nullopt;

    return expr.result();
}

void SpreadsheetGraph::createCellDepencencies(const CellIndex &at)
{
    addEdgeEntryIfNotExists(at);

    auto depends = mExpr.at(at).dependencies();
    for (const auto &cell : depends)
    {
        // TODO check if cell if in spreadsheet's bounds
        addEdgeEntryIfNotExists(cell);
        mForward.at(at).insert(cell);
        mReverse.at(cell).insert(at);
    }
}

void SpreadsheetGraph::purgeCellDependencies(const CellIndex &at)
{
    auto depends = mExpr.at(at).dependencies();
    for (const auto &cell : depends)
    {
        mForward.at(at).erase(cell);
        mReverse.at(cell).erase(at);
    }
}

void SpreadsheetGraph::addEdgeEntryIfNotExists(const CellIndex &at)
{
    if (mForward.find(at) == mForward.end()) {
        mAllVertices.insert(at);
        mForward.insert(std::make_pair(at, std::unordered_set<CellIndex>()));
    }
    if (mReverse.find(at) == mReverse.end()) {
        mReverse.insert(std::make_pair(at, std::unordered_set<CellIndex>()));
    }
}

void SpreadsheetGraph::propagateErrorDfs(const CellIndex &at, UsedMap &used)
{
    used[at] = true;
    mExpr.at(at).setError();
    for (const auto &to : mReverse.at(at))
    {
        if (!used[to])
            propagateErrorDfs(to, used);
    }
}

GraphCondensation::ComponentsList GraphCondensation::condensate(const SpreadsheetGraph::VertexSet &allVertices,
                                                     const SpreadsheetGraph::EdgeList &forward,
                                                     const SpreadsheetGraph::EdgeList &reverse)
{
    mForward = &forward;
    mReverse = &reverse;

    // sort in order
    mUsed.clear();
    std::vector<CellIndex> order;
    for (const auto &v : allVertices)
        if (!mUsed[v])
            dfsTopologicalSort(v, order);

    // visit reversed graph
    int curComponentNum = 0;
    mUsed.clear();
    ComponentsList result;
    for (auto it = order.rbegin(); it != order.rend(); ++it)
    {
        if (!mUsed[*it]) {
            result.emplace_back();
            dfsCondense(*it, result, curComponentNum++);
        }
    }

    mForward = nullptr;
    mReverse = nullptr;
    return result;
}

std::vector<CellIndex> GraphCondensation::topologicalSort(const SpreadsheetGraph::VertexSet &allVertices,
                                                          const SpreadsheetGraph::EdgeList &edges)
{
    mForward = &edges;
    mUsed.clear();

    std::vector<CellIndex> order;
    for (const auto &v : allVertices) {
        if (!mUsed[v])
            dfsTopologicalSort(v, order);
    }
    std::reverse(order.begin(), order.end());

    mForward = nullptr;
    return order;
}

bool GraphCondensation::isDAG(const GraphCondensation::ComponentsList &strongComponents) const
{
    return std::all_of(strongComponents.begin(), strongComponents.end(), [](const std::unordered_set<CellIndex> &s) {
        return s.size() == 1;
    });
}

void GraphCondensation::dfsTopologicalSort(const CellIndex &v, std::vector<CellIndex> &order)
{
    mUsed[v] = true;
    if (mForward->find(v) != mForward->end())
    {
        for (const auto &to : mForward->at(v))
        {
            if (!mUsed[to])
                dfsTopologicalSort(to, order);
        }
    }
    order.push_back(v);
}

void GraphCondensation::dfsCondense(const CellIndex &v, ComponentsList &result, int componentNum)
{
    mUsed[v] = true;
    result[componentNum].insert(v);
    if (mReverse->find(v) != mReverse->end())
    {
        for (const auto &to : mReverse->at(v))
        {
            if (!mUsed[to])
                dfsCondense(to, result, componentNum);
        }
    }
}
