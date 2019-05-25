#pragma once

#include <QtTest>
#include "spreadsheetgraph.h"

class SpreadsheetGraphTest : public QObject
{
    Q_OBJECT

private:
    SpreadsheetGraph::EdgeList getReverse(const SpreadsheetGraph::EdgeList &forward);

private Q_SLOTS:
    void testSetExpressionWithoutUpdateSimple();
    void testUpdateExpressionSimple();
    void testUpdateExpressionWithDependencies();

    void testGetReverse();
};
