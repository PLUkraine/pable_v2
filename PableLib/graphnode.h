#pragma once
#include <vector>
#include <memory>
#include "utils.h"

class IExpression;

struct GraphNode
{
    int id;
    std::shared_ptr<IExpression> expression;

    GraphNode(int id);
};

using NodeList = std::vector<GraphNode>;
using EdgeList = std::vector<std::vector<int>>;
