#pragma once

#include <QtTest>
#include "spreadsheetgraph.h"

std::string toString(const std::unordered_set<CellIndex> &v);

class SpreadsheetGraphTest : public QObject
{
    Q_OBJECT

private:
    SpreadsheetGraph::EdgeList getReverse(const SpreadsheetGraph::EdgeList &forward);
    SpreadsheetGraph::VertexSet extractAllVertices(const SpreadsheetGraph::EdgeList &forward);
    void verifyCondensation(const std::vector<std::unordered_set<CellIndex> > &actual,
                            const std::vector<std::unordered_set<CellIndex> > &expected);


private Q_SLOTS:
    void testSetExpressionWithoutUpdateSimple();
    void testUpdateExpressionSimple();
    void testUpdateExpressionWithDependencies();
    void testHasCell();

    void testGetReverse();
    void testGraphCondensationSimple();
    void testTopologicalSorting();
    void testIsDAG();
};
