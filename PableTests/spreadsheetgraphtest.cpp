#include "spreadsheetgraphtest.h"
#include "spreadsheetgraph.h"
#include "testutils.h"


void SpreadsheetGraphTest::testSetExpressionWithoutUpdateSimple()
{
    SpreadsheetGraph graph;

    graph.setExpressionWithoutUpdate(*CellIndex::str("$A1"), Expression::fromNumber(2));
    auto actual = graph.getValue(*CellIndex::str("$A1"));
    std::optional<int> expected = 0;
    QCOMPARE(actual, expected);

    graph.setExpressionWithoutUpdate(*CellIndex::str("$A2"), Expression::fromNumber(5));
    actual = graph.getValue(*CellIndex::str("$A2"));
    expected = 0;
    QCOMPARE(actual, expected);
    QCOMPARE(Expression::fromNumber(5), graph.getExpression(*CellIndex::str("$A2")));

    std::vector<Token> tokensList = {
        *CellIndex::str("$A1"), *CellIndex::str("$A2"), '-'
    };
    graph.setExpressionWithoutUpdate(*CellIndex::str("$A3"), Expression::fromTokens(tokensList));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = 0;
    QCOMPARE(actual, expected);
    QCOMPARE(Expression::fromTokens(tokensList), graph.getExpression(*CellIndex::str("$A3")));

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

    graph.updateExpression(*CellIndex::str("$A1"), Expression::fromNumber(3));
    auto actual = graph.getValue(*CellIndex::str("$A1"));
    std::optional<int> expected = 3;
    QCOMPARE(actual, expected);

    graph.updateExpression(*CellIndex::str("$A2"), Expression::fromNumber(4));
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

void SpreadsheetGraphTest::testUpdateExpressionWithDependencies()
{
    SpreadsheetGraph graph;

    // not dependent
    graph.updateExpression(*CellIndex::str("$A1"), Expression::fromNumber(2));
    graph.updateExpression(*CellIndex::str("$A2"), Expression::fromNumber(4));
    // middle
    graph.updateExpression(*CellIndex::str("$A3"), Expression::fromTokens({
                                                                              *CellIndex::str("$A1"),
                                                                              *CellIndex::str("$A2"),
                                                                              '-'
                                                                          }));
    auto actual = graph.getValue(*CellIndex::str("$A3"));
    std::optional<int> expected = -2;
    QCOMPARE(actual, expected);

    // dependent
    graph.updateExpression(*CellIndex::str("$A4"), Expression::fromTokens({
                                                                              *CellIndex::str("$A3"),
                                                                              10,
                                                                              '+'
                                                                          }));
    actual = graph.getValue(*CellIndex::str("$A4"));
    expected = 8;
    QCOMPARE(actual, expected);

    graph.updateExpression(*CellIndex::str("$A5"), Expression::fromTokens({
                                                                              *CellIndex::str("$A3"),
                                                                              10,
                                                                              '-'
                                                                          }));
    actual = graph.getValue(*CellIndex::str("$A5"));
    expected = -12;
    QCOMPARE(actual, expected);

    // update middle
    graph.updateExpression(*CellIndex::str("$A3"), Expression::fromTokens({
                                                                              *CellIndex::str("$A1"),
                                                                              *CellIndex::str("$A2"),
                                                                              '+'
                                                                          }));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = 6;
    QCOMPARE(actual, expected);
    //check independent
    actual = graph.getValue(*CellIndex::str("$A1"));
    expected = 2;
    QCOMPARE(actual, expected);
    actual = graph.getValue(*CellIndex::str("$A2"));
    expected = 2;
    QCOMPARE(actual, expected);
    // check dependent
    actual = graph.getValue(*CellIndex::str("$A4"));
    expected = 16;
    QCOMPARE(actual, expected);
    actual = graph.getValue(*CellIndex::str("$A5"));
    expected = -4;
    QCOMPARE(actual, expected);
}
