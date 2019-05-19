#ifndef SPREADSHEETGRAPH_H
#define SPREADSHEETGRAPH_H

#include <unordered_set>
#include "parser.h"


// interface first...
class SpreadsheetGraph
{
private:
    using EdgeList = std::unordered_map<CellIndex, std::unordered_set<CellIndex>>;
    std::unordered_map<CellIndex, Expression> mExpr;
    EdgeList mForward;
    EdgeList mReverse;

public:
    SpreadsheetGraph();
    void setExpressionWithoutUpdate(const CellIndex &at, const Expression &expression);
    void updateExpression(const CellIndex &atIndex, const Expression &expression)
    {
        setExpressionWithoutUpdate(atIndex, expression);
        update(atIndex);
    }

    void update(const CellIndex &/*atIndex*/) {}
    void updateAll() {}

    const Expression &getExpression(const CellIndex &at) const;
    std::optional<int> getValue(const CellIndex &at) const;

private:
    void createCellDepencencies(const CellIndex &at);
    void purgeCellDependencies(const CellIndex &at);
    void addEdgeEntryIfNotExists(const CellIndex &at);
};

#endif // SPREADSHEETGRAPH_H
