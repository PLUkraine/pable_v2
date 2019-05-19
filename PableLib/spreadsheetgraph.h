#ifndef SPREADSHEETGRAPH_H
#define SPREADSHEETGRAPH_H

#include "parser.h"


// interface first...
class SpreadsheetGraph
{
public:
    SpreadsheetGraph();
    void setExpression(const CellIndex &/*atIndex*/, const Expression &/*expression*/) {}

    void update(const CellIndex &/*atIndex*/) {}
    void updateAll() {}

    const Expression &getExpression(const CellIndex &/*atIndex*/) const
    {
        static Expression _stub;
        return _stub;
    }
    std::optional<int> getValue(const CellIndex &/*atIndex*/) const {return 0;}
};

#endif // SPREADSHEETGRAPH_H
