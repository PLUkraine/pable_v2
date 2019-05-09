#include "graph.h"

int Graph::MAX_NODES = 4096;

Graph::Graph()
{
    mNodes.reserve(MAX_NODES);
    for (int i=0; i<MAX_NODES; ++i)
    {
        mNodes[i] = {i, 0};
    }

    mForw.assign(MAX_NODES, std::vector<int>());
    mBack.assign(MAX_NODES, std::vector<int>());
}

void Graph::setValue(int id, const GraphNodeValue &newValue)
{
    mNodes[id].value = newValue;
    recompute();
}

GraphNodeValue Graph::value(int id) const
{
    return mNodes[id].value;
}

void Graph::recompute()
{
    // TODO topological sorting
}
