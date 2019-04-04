#include "doubleliteral.h"
#include <QtGlobal>

namespace Pable {
namespace Tokens {

DoubleLiteral::DoubleLiteral(double value)
    : Token(Type::Literal),
      mValue(value)
{
}

double DoubleLiteral::value() const
{
    return mValue;
}

bool DoubleLiteral::operator==(const DoubleLiteral &other) const
{
    return qFuzzyCompare(mValue, other.mValue);
}

std::shared_ptr<DoubleLiteral> doubleLiteral(double value)
{
    return std::make_shared<DoubleLiteral>(value);
}

} // namespace Tokens
} // namespace Pable
