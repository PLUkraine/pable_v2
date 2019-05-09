#include "graph.h"
#include "expression.h"

int Graph::MAX_NODES = 4096;

Graph::Graph()
    : mLoopDetector(MAX_NODES)
{
    initializeGraph();
}

bool Graph::setValue(int id, std::shared_ptr<IExpression> newExpression)
{
    auto oldExpression = mNodes[id].expression;
    mNodes[id].expression = newExpression;
    // if not valid, reset oldValue
    if (!validate(id)) {
        mNodes[id].expression = oldExpression;
        return false;
    }

    recompute(id);
    return true;
}

Number Graph::value(int id) const
{
    assert(mNodes[id].expression->hasResult());
    return mNodes[id].expression->result();
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
    // 1. Topological sort on forward starting from our node
    // 2. Compute all values according to the sort order
    // 3. Topological sort on backward starting from our node
    // 4. Compute all values according to the sort order
    // 5. ???
    // 6. ALL DONE
}

void Graph::initializeGraph()
{
    // assign all nodes to 0
    mNodes.reserve(MAX_NODES);
    for (int i=0; i<MAX_NODES; ++i)
    {
        mNodes.emplace_back(i);
    }

    // no edges, yet
    mForw.assign(MAX_NODES, std::vector<int>());
    mBack.assign(MAX_NODES, std::vector<int>());
}
