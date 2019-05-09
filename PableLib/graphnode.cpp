#include "expression.h"


GraphNode::GraphNode(int id)
    : id(id),
      expression(nullExpression())
{
}
