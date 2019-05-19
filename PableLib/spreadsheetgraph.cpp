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
