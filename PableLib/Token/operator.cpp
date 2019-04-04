#include "operator.h"

namespace Pable {
namespace Tokens {

std::shared_ptr<Operator> plusOp = std::make_shared<Operator>(2, '+');
std::shared_ptr<Operator> minusOp = std::make_shared<Operator>(2, '-');

Operator::Operator(int precedence, char symbol)
    : Token(Type::Operator),
      mPrecedence(precedence),
      mSymbol(symbol)
{
}

Operator::Operator(const Operator &other)
    : Operator(other.mPrecedence, other.mSymbol)
{

}

int Operator::precedence() const
{
    return mPrecedence;
}

char Operator::symbol() const
{
    return mSymbol;
}

Operator &Operator::operator=(Operator other)
{
    swap(*this, other);
    return *this;
}

bool Operator::operator==(const Operator &other) const
{
    return mSymbol == other.mSymbol;
}

void swap(Operator &first, Operator &second)
{
    using std::swap;
    swap(first.mPrecedence, second.mPrecedence);
    swap(first.mSymbol, second.mSymbol);
}



} // namespace Tokens
} // namespace Pable
