#include "graphtest.h"
#include "graphloopdetector.h"

void GraphTest::testLoopDetector()
{
    GraphLoopDetector detector(10);
    {
        EdgeList g(10, std::vector<int>());
        g[0] = {1, 2};
        g[1] = {2, 3};
        g[2] = {};
        g[3] = {4};

        QVERIFY(not detector.hasCycle(g, 0));
    }
    {
        EdgeList g(10, std::vector<int>());
        g[0] = {1, 2};
        g[1] = {2, 3};
        g[2] = {3, 1};
        g[3] = {4};

        QVERIFY(detector.hasCycle(g, 0));
    }
    {
        EdgeList g(10, std::vector<int>());
        g[0] = {1};
        g[1] = {};
        g[2] = {3};
        g[3] = {2};

        QVERIFY(not detector.hasCycle(g, 0));
        QVERIFY(detector.hasCycle(g, 2));
    }
}
