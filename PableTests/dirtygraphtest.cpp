#include "dirtygraphtest.h"

#include "dirtygraph.h"


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
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 1);
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
