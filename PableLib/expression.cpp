#include "expression.h"


void Graph::changeExpression(GraphId where, ExpressionPtr to)
{
    GraphDependencyUpdator().updateDependencies(mForward, mBackward, to->dependencies());

    auto forwardTopologySort = TopologySorting().sortBackwards(mForward, where);
    for (int id : forwardTopologySort)
    {
        mCells[id]->evaluate(mCells);
    }

    // TODO update dependent nodes
    if ((mCells[where]->result()).index() == 0)
    {
        // VALUE HAS NO ERRORS
    }
    else
    {
        // ERROR IN NODE; SET ERROR TO ALL DEPENDENT NODES
    }
}
