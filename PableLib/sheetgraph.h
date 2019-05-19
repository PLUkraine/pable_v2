#ifndef SHEETGRAPH_H
#define SHEETGRAPH_H

#include "parser.h"

class SheetGraph
{
private:
    using Edges = std::unordered_map<CellIndex, std::vector<CellIndex>>;
    Edges mForward;
    Edges mReverse;
    using Vertices = std::unordered_map<CellIndex, Expression>;
    Vertices mExpressions;

public:
    SheetGraph();
};

#endif // SHEETGRAPH_H
