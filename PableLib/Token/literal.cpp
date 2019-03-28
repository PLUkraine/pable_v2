#include "literal.h"
#include <QtGlobal>

namespace Pable {
namespace Tokens {

Literal::Literal(double value)
    : Token(Type::Literal),
      mValue(value)
{
}

double Literal::value() const
{
    return mValue;
}

bool Literal::operator==(const Literal &other) const
{
    return qFuzzyCompare(mValue, other.mValue);
}

} // namespace Tokens
} // namespace Pable
