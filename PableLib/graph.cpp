#include "graph.h"
#include "utils.h"

int Graph::MAX_NODES = 4096;

Graph::Graph()
    : mLoopDetector(MAX_NODES)
{
    initializeGraph();
}

bool Graph::setValue(int id, const GraphNodeValue &newValue)
{
    int oldValue = mNodes[id].value;
    mNodes[id].value = newValue;
    // if not valid, reset oldValue
    if (!validate(id)) {
        mNodes[id].value = oldValue;
        return false;
    }

    recompute(id);
    return true;
}

GraphNodeValue Graph::value(int id) const
{
    return mNodes[id].value;
}

bool Graph::validate(int idChanged)
{
    // TODO cycle detection
    return mLoopDetector.hasCycle(mForw, idChanged);
}

void Graph::recompute(int idChanged)
{
    // TODO topological sorting
    UNUSED_PARAM(idChanged);
}

void Graph::initializeGraph()
{
    // assign all nodes to 0
    mNodes.reserve(MAX_NODES);
    for (int i=0; i<MAX_NODES; ++i)
    {
        mNodes[i] = {i, 0};
    }

    // no edges, yet
    mForw.assign(MAX_NODES, std::vector<int>());
    mBack.assign(MAX_NODES, std::vector<int>());
}
