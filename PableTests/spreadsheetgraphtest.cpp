#include "spreadsheetgraphtest.h"
#include "testutils.h"

SpreadsheetGraph::EdgeList SpreadsheetGraphTest::getReverse(const SpreadsheetGraph::EdgeList &forward)
{
    SpreadsheetGraph::EdgeList reverse;
    for (auto it : forward)
    {
        CellIndex v = it.first;
        for (auto to : it.second)
        {
            reverse[to].insert(v);
        }
    }
    return reverse;
}

SpreadsheetGraph::VertexSet SpreadsheetGraphTest::extractAllVertices(const SpreadsheetGraph::EdgeList &forward)
{
    SpreadsheetGraph::VertexSet answer;
    for (const auto entry : forward)
    {
        answer.insert(entry.first);
        answer.insert(entry.second.begin(), entry.second.end());
    }
    return answer;
}

void SpreadsheetGraphTest::verifyCondensation(const std::vector<std::unordered_set<CellIndex>> &actual,
                                              const std::vector<std::unordered_set<CellIndex>> &expected)
{
    QCOMPARE(actual.size(), expected.size());

    for (auto actualSet : actual) {
        auto it = std::find(expected.begin(), expected.end(), actualSet);
        if (it == actual.end()) {
            const auto message = toString(actualSet) + " is not in the expected set";
            QFAIL(message.c_str());
        }
    }
}

void SpreadsheetGraphTest::testSetExpressionWithoutUpdateSimple()
{
    SpreadsheetGraph graph;

    graph.setExpressionWithoutUpdate(*CellIndex::str("$A1"), Expression::fromNumber(2));
    auto actual = graph.getValue(*CellIndex::str("$A1"));
    std::optional<int> expected = std::nullopt;
    QCOMPARE(actual, expected);

    graph.setExpressionWithoutUpdate(*CellIndex::str("$A2"), Expression::fromNumber(5));
    actual = graph.getValue(*CellIndex::str("$A2"));
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(Expression::fromNumber(5), graph.getExpression(*CellIndex::str("$A2")));

    std::vector<Token> tokensList = {
        *CellIndex::str("$A1"), *CellIndex::str("$A2"), '-'
    };
    graph.setExpressionWithoutUpdate(*CellIndex::str("$A3"), Expression::fromTokens(tokensList));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(Expression::fromTokens(tokensList), graph.getExpression(*CellIndex::str("$A3")));

    // evaluate now
    graph.updateAll();
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = -3;
    QCOMPARE(actual, expected);

    // update expression
    tokensList = {
        *CellIndex::str("$A1"), *CellIndex::str("$A2"), '+'
    };
    graph.setExpressionWithoutUpdate(*CellIndex::str("$A3"), Expression::fromTokens(tokensList));
    actual = graph.getValue(*CellIndex::str("$A3"));
    expected = std::nullopt;
    QCOMPARE(actual, expected);

    // reevaluate now
    graph.updateAll();
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
    expected = 4;
    QCOMPARE(actual, expected);
    // check dependent
    actual = graph.getValue(*CellIndex::str("$A4"));
    expected = 16;
    QCOMPARE(actual, expected);
    actual = graph.getValue(*CellIndex::str("$A5"));
    expected = -4;
    QCOMPARE(actual, expected);
}

void SpreadsheetGraphTest::testHasCell()
{
    SpreadsheetGraph graph;
    QVERIFY(!graph.hasCell(CellIndex(0, 0)));
    QCOMPARE(graph.getValue(CellIndex(0, 0)), 0);
    QVERIFY(!graph.hasCell(CellIndex(0, 0)));

    graph.setExpressionWithoutUpdate(CellIndex(0,0), Expression::fromNumber(1));
    QVERIFY(graph.hasCell(CellIndex(0,0)));

    // set invalid expression
    QVERIFY(!graph.hasCell(CellIndex(0,1)));
    QVERIFY(!graph.hasCell(CellIndex(0,2)));
    QVERIFY(!graph.hasCell(CellIndex(0,3)));
    graph.setExpressionWithoutUpdate(CellIndex(0,1),
                                     Expression::fromTokens({
                                                                CellIndex(0,2),
                                                                CellIndex(0,3),
                                                            }));
    QVERIFY(graph.hasCell(CellIndex(0,1)));
    QVERIFY(graph.hasCell(CellIndex(0,2)));
    QVERIFY(graph.hasCell(CellIndex(0,3)));

    // set valid expression
    graph.setExpressionWithoutUpdate(CellIndex(0,4),
                                     Expression::fromTokens({
                                                                CellIndex(0,5),
                                                                CellIndex(0,6),
                                                                '+'
                                                            }));
    QVERIFY(graph.hasCell(CellIndex(0,4)));
    QVERIFY(graph.hasCell(CellIndex(0,5)));
    QVERIFY(graph.hasCell(CellIndex(0,6)));
}

void SpreadsheetGraphTest::testGetReverse()
{
    SpreadsheetGraph::EdgeList forward = {
        {CellIndex(0, 1), {
            CellIndex(0, 2),
            CellIndex(0, 3),
        }},
    };
    SpreadsheetGraph::EdgeList reverse = getReverse(forward);
    SpreadsheetGraph::EdgeList expected = {
        {CellIndex(0, 2), {
             CellIndex(0, 1),
         }},

         {CellIndex(0, 3), {
              CellIndex(0, 1),
          }},
    };

    forward = {
        {CellIndex(0, 1), {
             CellIndex(0, 2),
             CellIndex(0, 4),
         }},
        {CellIndex(0, 2), {
             CellIndex(0, 3),
             CellIndex(0, 4),
         }},
        {CellIndex(0, 4), {
            CellIndex(0, 2),
         }},
    };
    expected = {
        {CellIndex(0, 2), {
             CellIndex(0, 1),
             CellIndex(0, 4)
         }},
        {CellIndex(0, 4), {
             CellIndex(0, 1),
             CellIndex(0, 2),
         }},
        {CellIndex(0, 3), {
            CellIndex(0, 2),
         }},
    };
    reverse = getReverse(forward);
    QCOMPARE(reverse, expected);
}

void SpreadsheetGraphTest::testGraphCondensationSimple()
{
    GraphCondensation condensation;

    SpreadsheetGraph::EdgeList forward = {
        {CellIndex(0, 1), {
            CellIndex(0, 2),
            CellIndex(0, 3),
        }},
    };
    auto reverse = getReverse(forward);
    auto vertices = extractAllVertices(forward);
    auto actual = condensation.condensate(vertices, forward, reverse);
    verifyCondensation(actual, {
                           {CellIndex(0, 1)},
                           {CellIndex(0, 2)},
                           {CellIndex(0, 3)},
                       });

    forward = {
        {CellIndex(0, 1), {
             CellIndex(0, 2),
         }},
        {CellIndex(0, 2), {
             CellIndex(0, 3),
         }},
        {CellIndex(0, 3), {
             CellIndex(0, 2),
             CellIndex(0, 4)
         }},
    };
    reverse = getReverse(forward);
    actual = condensation.condensate(vertices, forward, reverse);
    verifyCondensation(actual, {
                           {CellIndex(0, 1)},
                           {CellIndex(0, 2), CellIndex(0, 3)},
                           {CellIndex(0, 4)},
                       });
}

void SpreadsheetGraphTest::testTopologicalSorting()
{
    GraphCondensation condensation;

    SpreadsheetGraph::EdgeList forward = {
        {CellIndex(0, 1), {
             CellIndex(0, 2),
             CellIndex(0, 3),
         }},
        {CellIndex(0, 2), {
             CellIndex(0, 3),
             CellIndex(0, 4),
         }},
        {CellIndex(0, 4), {
             CellIndex(0, 5),
         }},
        {CellIndex(0, 6), {
             CellIndex(0, 7),
         }},
    };
    auto vertices = extractAllVertices(forward);
    auto actual = condensation.topologicalSort(vertices, forward);
    std::vector<CellIndex> expected = {
        CellIndex(0, 6), CellIndex(0, 7),
        CellIndex(0, 1), CellIndex(0, 2), CellIndex(0, 4), CellIndex(0, 5), CellIndex(0, 3),
    };
    QCOMPARE(actual, expected);
}

void SpreadsheetGraphTest::testIsDAG()
{
    GraphCondensation condensation;

    SpreadsheetGraph::EdgeList forward = {
        {CellIndex(0, 1), {
             CellIndex(0, 2),
         }},
        {CellIndex(0, 2), {
             CellIndex(0, 3),
         }},
        {CellIndex(0, 3), {
             CellIndex(0, 2),
             CellIndex(0, 4)
         }},
    };
    auto reverse = getReverse(forward);
    auto vertices = extractAllVertices(forward);
    auto components = condensation.condensate(vertices, forward, reverse);
    QVERIFY(!condensation.isDAG(components));

    forward = {
        {CellIndex(0, 1), {
             CellIndex(0, 2),
             CellIndex(0, 3),
         }},
        {CellIndex(0, 2), {
             CellIndex(0, 3),
             CellIndex(0, 4),
         }},
        {CellIndex(0, 4), {
             CellIndex(0, 5),
         }},
        {CellIndex(0, 6), {
             CellIndex(0, 7),
         }},
    };
    reverse = getReverse(forward);
    vertices = extractAllVertices(forward);
    components = condensation.condensate(vertices, forward, reverse);
    QVERIFY(condensation.isDAG(components));
}

std::string toString(const std::unordered_set<CellIndex> &v)
{
    std::string res;
    for (const auto &c : v) {
        res += c.toString();
        res += " ";
    }
    return res;
}
