#include "spreadsheetgraphtest.h"
#include "spreadsheetgraph.h"
#include "testutils.h"


void SpreadsheetGraphTest::testSetExpressionWithoutUpdateSimple()
{
    SpreadsheetGraph graph;

    graph.setExpressionWithoutUpdate(*CellIndex::str("$A1"), Expression::fromTokens({2}));
    auto actual = graph.getValue(*CellIndex::str("$A1"));
    std::optional<int> expected = 0;
    QCOMPARE(actual, expected);

    graph.setExpressionWithoutUpdate(*CellIndex::str("$A2"), Expression::fromTokens({5}));
    actual = graph.getValue(*CellIndex::str("$A2"));
    expected = 0;
    QCOMPARE(actual, expected);

    std::vector<Token> tokensList = {
        *CellIndex::str("$A1"), *CellIndex::str("$A2"), '-'
    };
    graph.setExpressionWithoutUpdate(*CellIndex::str("$A3"), Expression::fromTokens(tokensList));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = 0;
    QCOMPARE(actual, expected);

    // evaluate now
    graph.update(*CellIndex::str("$A3"));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = -3;
    QCOMPARE(actual, expected);

    // update expression
    tokensList = {
        *CellIndex::str("$A1"), *CellIndex::str("$A2"), '+'
    };
    graph.setExpressionWithoutUpdate(*CellIndex::str("$A3"), Expression::fromTokens(tokensList));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = -3;
    QCOMPARE(actual, expected);

    // reevaluate now
    graph.update(*CellIndex::str("$A3"));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = 7;
    QCOMPARE(actual, expected);
}

void SpreadsheetGraphTest::testUpdateExpressionSimple()
{
    SpreadsheetGraph graph;

    graph.updateExpression(*CellIndex::str("$A1"), Expression::fromTokens({3}));
    auto actual = graph.getValue(*CellIndex::str("$A1"));
    std::optional<int> expected = 3;
    QCOMPARE(actual, expected);

    graph.updateExpression(*CellIndex::str("$A2"), Expression::fromTokens({4}));
    actual = graph.getValue(*CellIndex::str("$A2"));
    expected = 4;
    QCOMPARE(actual, expected);

    graph.updateExpression(*CellIndex::str("$A3"), Expression::fromTokens({
                                                                              *CellIndex::str("$A1"),
                                                                              *CellIndex::str("$A2"),
                                                                              '-'
                                                                          }));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = -1;
    QCOMPARE(actual, expected);

    graph.updateExpression(*CellIndex::str("$A3"), Expression::fromTokens({
                                                                              *CellIndex::str("$A1"),
                                                                              *CellIndex::str("$A2"),
                                                                              '-'
                                                                          }));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = -1;
    QCOMPARE(actual, expected);

    graph.updateExpression(*CellIndex::str("$A3"), Expression::fromTokens({
                                                                              *CellIndex::str("$A1"),
                                                                              *CellIndex::str("$A2"),
                                                                              '+'
                                                                          }));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = 7;
    QCOMPARE(actual, expected);
}
