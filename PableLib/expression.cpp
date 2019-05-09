#include "expression.h"

namespace
{
    PExpression __nullExpression = std::make_shared<NullExpression>();
}

PExpression nullExpression()
{
    return __nullExpression;
}
