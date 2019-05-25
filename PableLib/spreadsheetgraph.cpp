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
    update(atIndex);
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
        mForward.insert(std::make_pair(at, std::unordered_set<CellIndex>()));
    }
    if (mReverse.find(at) == mReverse.end()) {
        mReverse.insert(std::make_pair(at, std::unordered_set<CellIndex>()));
    }
}

GraphCondensation::GraphCondensation(const SpreadsheetGraph::EdgeList &forward,
                                     const SpreadsheetGraph::EdgeList &reverse)
    : mForward(forward),
      mReverse(reverse)
{

}

GraphCondensation::Result GraphCondensation::condenceFromVertex(const CellIndex &start)
{
    // sort in order
    mUsed.clear();
    std::vector<CellIndex> order;
    dfsTopologicalSort(start, order);

    // visit reversed graph
    int curComponentNum = 0;
    mUsed.clear();
    Result result;
    for (auto it = order.rbegin(); it != order.rend(); ++it)
    {
        if (!mUsed[*it]) {
            result.components.emplace_back();
            dfsCondense(*it, result, curComponentNum++);
        }
    }

    return result;
}

void GraphCondensation::dfsTopologicalSort(const CellIndex &v, std::vector<CellIndex> &order)
{
    mUsed[v] = true;
    if (mForward.find(v) != mForward.end())
    {
        for (const auto &to : mForward.at(v))
        {
            if (!mUsed[to])
                dfsTopologicalSort(to, order);
        }
    }
    order.push_back(v);
}

void GraphCondensation::dfsCondense(const CellIndex &v, GraphCondensation::Result &result, int componentNum)
{
    mUsed[v] = true;
    result.components[componentNum].insert(v);
    if (mReverse.find(v) != mReverse.end())
    {
        for (const auto &to : mReverse.at(v))
        {
            if (!mUsed[to])
                dfsCondense(to, result, componentNum);
        }
    }
}
