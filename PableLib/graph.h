#pragma once
#include "graphnode.h"
#include "graphloopdetector.h"

class IExpression;

class Graph
{
private:
    static int MAX_NODES;
    GraphLoopDetector mLoopDetector;
    NodeList mNodes;
    EdgeList mForw;
    EdgeList mBack;

public:
    Graph();

    bool setValue(int id, std::shared_ptr<IExpression> newExpression);
    Number value(int id) const;

private:
    bool validate(int idChanged);
    void recompute(int idChanged);

    void initializeGraph();
};
