#ifndef SPREADSHEETGRAPH_H
#define SPREADSHEETGRAPH_H

#include <unordered_set>
#include "parser.h"


// interface first...
class SpreadsheetGraph
{
public:
    using EdgeList = std::unordered_map<CellIndex, std::unordered_set<CellIndex>>;
private:
    std::unordered_map<CellIndex, Expression> mExpr;
    EdgeList mForward;
    EdgeList mReverse;

public:
    SpreadsheetGraph();
    void setExpressionWithoutUpdate(const CellIndex &at, const Expression &expression);
    void updateExpression(const CellIndex &atIndex, const Expression &expression);

    void update(const CellIndex &/*atIndex*/) {}
    void updateAll() {}

    const Expression &getExpression(const CellIndex &at) const;
    std::optional<int> getValue(const CellIndex &at) const;

private:
    void createCellDepencencies(const CellIndex &at);
    void purgeCellDependencies(const CellIndex &at);
    void addEdgeEntryIfNotExists(const CellIndex &at);
};

class GraphCondensation
{
private:
    using UsedVec = std::unordered_map<CellIndex, bool>;
    UsedVec mUsed;
    const SpreadsheetGraph::EdgeList &mForward;
    const SpreadsheetGraph::EdgeList &mReverse;

public:
    struct Result
    {
        std::vector<std::unordered_set<CellIndex>> components;
    };

    GraphCondensation(const SpreadsheetGraph::EdgeList &forward,
                      const SpreadsheetGraph::EdgeList &reverse);
    Result condenceFromVertex(const CellIndex &start);

private:
    void dfsTopologicalSort(const CellIndex &v, std::vector<CellIndex> &order);
    void dfsCondense(const CellIndex &v, Result &result, int componentNum);
};

#endif // SPREADSHEETGRAPH_H
