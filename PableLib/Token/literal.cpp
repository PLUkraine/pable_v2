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

} // namespace Tokens
} // namespace Pable
