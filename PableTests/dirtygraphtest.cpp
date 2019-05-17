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
