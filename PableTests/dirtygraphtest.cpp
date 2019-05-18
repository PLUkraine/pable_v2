#include "dirtygraphtest.h"
#include "dirtygraph.h"
#include "testutils.h"


void DirtyGraphTest::testClear()
{
    const int COUNT = 5;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 1);
    graph.addEdge(4, 0);

    graph.clear();
    for (int i=0; i<COUNT; ++i)
        QCOMPARE(graph.countDependencies(i), 0);
}

void DirtyGraphTest::testCycleDetection()
{
    const int COUNT = 8;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    QCOMPARE(graph.getValue(0), 0);
    graph.addEdge(1, 2);
    QCOMPARE(graph.getValue(0), 0);
    graph.addEdge(2, 3);
    QCOMPARE(graph.getValue(0), 0);
    graph.addEdge(3, 1);
    QCOMPARE(graph.getValue(0), std::nullopt);
    auto res = graph.detectForwardCycle(0);
    auto res2 = graph.detectReverseCycle(2);
    auto res3 = graph.detectReverseCycle(3);
    QCOMPARE(res.size(), 3u);
    QCOMPARE(res2.size(), 3u);
    QCOMPARE(res3.size(), 3u);

    graph.clear();
    graph.addEdge(0, 1);
    graph.addEdge(0, 4);
    graph.addEdge(0, 6);
    graph.addEdge(1, 4);
    graph.addEdge(4, 6);
    graph.addEdge(2, 3);
    graph.addEdge(3, 4);
    for (int i=0; i<COUNT; ++i)
    {
        res = graph.detectForwardCycle(i);
        res2 = graph.detectReverseCycle(i);
        QVERIFY2(res.empty(), (QString::number(i) + " node").toStdString().c_str());
        QVERIFY2(res2.empty(), (QString::number(i) + " node").toStdString().c_str());
    }


}

void DirtyGraphTest::testLongGraphAddEdge()
{
    const int COUNT = 8;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(0, 2);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(6, 7);
    graph.addEdge(4, 7);

    auto nodeSet = {0, 1, 2, 3, 4, 5, 6, 7};
    auto actual = graph.getValues(nodeSet);
    std::vector<std::optional<int>> expected = {0, 0, 0, 0, 0, 0, 0, 0};
    QCOMPARE(actual, expected);

    graph.addEdge(4, 3);
    actual = graph.getValues(nodeSet);
    expected = {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, 0, 0, 0};
    QCOMPARE(actual, expected);

    graph.removeEdge(4, 3);
    graph.reevaluate(3);
    actual = graph.getValues(nodeSet);
    expected = {0, 0, 0, 0, 0, 0, 0, 0};
    QCOMPARE(actual, expected);
}

void DirtyGraphTest::testInvalidate()
{
    const int COUNT = 10;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(2, 3);
    graph.addEdge(0, 4);

    graph.addEdge(0, 5);
    graph.addEdge(0, 6);
    graph.addEdge(5, 7);
    graph.addEdge(6, 7);

    auto nodeSet = {0, 1, 2, 3, 4, 5, 6, 7};
    graph.setInvalid(0);
    auto actual = graph.getValues(nodeSet);
    std::vector<std::optional<int>> expected = {std::nullopt, 0, 0, 0, 0, 0, 0, 0};
    QCOMPARE(actual, expected);

    graph.setInvalid(3);
    actual = graph.getValues(nodeSet);
    expected = {std::nullopt, 0, std::nullopt, std::nullopt, 0, 0, 0, 0};
    QCOMPARE(actual, expected);

    graph.setInvalid(1);
    actual = graph.getValues(nodeSet);
    expected = {std::nullopt, std::nullopt, std::nullopt, std::nullopt, 0, 0, 0, 0};
    QCOMPARE(actual, expected);

    graph.setInvalid(7);
    actual = graph.getValues(nodeSet);
    expected = {std::nullopt, std::nullopt, std::nullopt, std::nullopt, 0, std::nullopt, std::nullopt, std::nullopt};
    QCOMPARE(actual, expected);
}

void DirtyGraphTest::testCycleInvalidation()
{
    const int COUNT = 8;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 1);
    graph.addEdge(3, 4);

    auto actual = graph.getValues({0, 1, 2, 3, 4});
    std::vector<std::optional<int>> expected = {std::nullopt, std::nullopt, std::nullopt, std::nullopt, 0};
    QCOMPARE(actual, expected);
}

void DirtyGraphTest::testSetValueSimple()
{
    const int COUNT = 8;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 2);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(2, 4);

    const std::vector<int> setOfNodes = {0, 1, 2, 3, 4};
    QVERIFY(graph.setValue(2, 10));
    auto actual = graph.getValues(setOfNodes);
    std::vector<std::optional<int>> expected = {10, 10, 10, 0, 0};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(4, -10));
    actual = graph.getValues(setOfNodes);
    expected = {-10, -10, -10, 0, -10};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(1, 20));
    actual = graph.getValues(setOfNodes);
    expected = {-10, 20, -10, 0, -10};
    QCOMPARE(actual, expected);

    graph.setInvalid(2);
    QVERIFY(!graph.setValue(2, -20));
    actual = graph.getValues(setOfNodes);
    expected = {std::nullopt, std::nullopt, std::nullopt, 0, -10};
    QCOMPARE(actual, expected);
}

void DirtyGraphTest::testSetValueLong()
{
    const int COUNT = 8;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(0, 2);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(6, 7);
    graph.addEdge(4, 7);

    const std::vector<int> setOfNodes = {0, 1, 2, 3, 4, 5, 6, 7};
    QVERIFY(graph.setValue(2, 1));
    auto actual = graph.getValues(setOfNodes);
    std::vector<std::optional<int>> expected = {2, 1, 1, 0, 0, 0, 0, 0};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(3, 5));
    actual = graph.getValues(setOfNodes);
    expected = {10, 5, 5, 5, 0, 0, 0, 0};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(4, 5));
    actual = graph.getValues(setOfNodes);
    expected = {10, 5, 5, 5, 5, 0, 0, 0};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(6, 10));
    actual = graph.getValues(setOfNodes);
    expected = {20, 10, 10, 10, 10, 10, 10, 0};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(7, -10));
    actual = graph.getValues(setOfNodes);
    expected = {-40, -20, -20, -20, -20, -10, -10, -10};
    QCOMPARE(actual, expected);

    QVERIFY(graph.setValue(3, 2));
    actual = graph.getValues(setOfNodes);
    expected = {4, 2, 2, 2, -20, -10, -10, -10};
    QCOMPARE(actual, expected);

    graph.setInvalid(4);
    actual = graph.getValues(setOfNodes);
    expected = {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, -10, -10, -10};
    QCOMPARE(actual, expected);

    QVERIFY(!graph.setValue(4, 2));
    actual = graph.getValues(setOfNodes);
    expected = {std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, -10, -10, -10};
    QCOMPARE(actual, expected);
}

void DirtyGraphTest::testReevaluate()
{
    const int COUNT = 8;
    DirtyGraph graph(COUNT);
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(0, 2);
    graph.addEdge(3, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(6, 7);
    graph.addEdge(4, 7);

    const std::vector<int> setOfNodes = {0, 1, 2, 3, 4, 5, 6, 7};
    graph.setValue(7, -10);
    graph.setValue(3, 2);

    auto result = graph.reevaluate(3);
    auto actual = graph.getValues(setOfNodes);
    std::vector<std::optional<int>> expected = {0, 0, 0, 0, 0, 0, 0, 0};
    QCOMPARE(actual, expected);
    QCOMPARE(result, 0);

    graph.setInvalid(3);
    result = graph.reevaluate(3);
    actual = graph.getValues(setOfNodes);
    expected = {0, 0, 0, 0, 0, 0, 0, 0};
    QCOMPARE(actual, expected);
    QCOMPARE(result, 0);
}
