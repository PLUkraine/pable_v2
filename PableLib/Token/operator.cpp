#include "operator.h"

namespace Pable {
namespace Tokens {

Operator::Operator(std::function<double (double, double)> func, int precedence, char symbol)
    : Token(Type::Operator),
      mFunc(func),
      mPrecedence(precedence),
      mSymbol(symbol)
{
}

Operator::Operator(const Operator &other)
    : Operator(other.mFunc, other.mPrecedence, other.mSymbol)
{

}

double Operator::apply(double op1, double op2) const
{
    return mFunc(op1, op2);
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
    swap(first.mFunc, second.mFunc);
    swap(first.mPrecedence, second.mPrecedence);
    swap(first.mSymbol, second.mSymbol);
}



} // namespace Tokens
} // namespace Pable
