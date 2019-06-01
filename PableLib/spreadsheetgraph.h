#ifndef SPREADSHEETGRAPH_H
#define SPREADSHEETGRAPH_H

#include <unordered_set>
#include "parser.h"

// interface first...
class SpreadsheetGraph : public ExpressionContext
{
public:
    using EdgeList = std::unordered_map<CellIndex, std::unordered_set<CellIndex>>;
    using VertexSet = std::unordered_set<CellIndex>;
    using UsedMap = std::unordered_map<CellIndex, bool>;
private:
    std::unordered_map<CellIndex, Expression> mExpr;
    VertexSet mAllVertices;
    EdgeList mForward;
    EdgeList mReverse;

public:
    void setExpressionWithoutUpdate(const CellIndex &at, const Expression &expression);
    void updateExpression(const CellIndex &atIndex, const Expression &expression);

    void updateAll();

    const Expression &getExpression(const CellIndex &at) const;
    std::optional<int> getValue(const CellIndex &at) const;
    bool hasCell(const CellIndex &at) const;
    void clear(const CellIndex &at);

private:
    void createCellDepencencies(const CellIndex &at);
    void purgeCellDependencies(const CellIndex &at);
    void addEdgeEntryIfNotExists(const CellIndex &at);

    void propagateErrorDfs(const CellIndex &at, UsedMap &used, Expression::Error errorType);
};

class GraphCondensation
{
private:
    std::unordered_map<CellIndex, bool> mUsed;

    SpreadsheetGraph::EdgeList const *mForward;
    SpreadsheetGraph::EdgeList const *mReverse;

public:
    using ComponentsList = std::vector<std::unordered_set<CellIndex>>;

    ComponentsList condensate(const SpreadsheetGraph::VertexSet &allVertices,
                              const SpreadsheetGraph::EdgeList &forward,
                              const SpreadsheetGraph::EdgeList &reverse);
    std::vector<CellIndex> topologicalSort(const SpreadsheetGraph::VertexSet &allVertices,
                                           const SpreadsheetGraph::EdgeList &edges);
    bool isDAG(const ComponentsList &strongComponents) const;

private:
    void dfsTopologicalSort(const CellIndex &v, std::vector<CellIndex> &order);
    void dfsCondense(const CellIndex &v, ComponentsList &result, int componentNum);
};

#endif // SPREADSHEETGRAPH_H
