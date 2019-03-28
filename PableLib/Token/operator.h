#pragma once

#include "Token/token.h"
#include <functional>

namespace Pable {
namespace Tokens {

class Operator : public Token
{
public:
    Operator(std::function<double(double, double)> func, int precedence, char symbol);
    Operator(const Operator& other);

    double apply(double op1, double op2) const;
    int precedence() const;
    char symbol() const;

    friend void swap(Operator& first, Operator& second);
    Operator &operator=(Operator other);
    bool operator==(const Operator &other) const;

private:
    std::function<double(double, double)> mFunc;
    int mPrecedence;
    char mSymbol;
};

const Operator plusOp([](double a, double b){return a+b;}, 2, '+');
const Operator minusOp([](double a, double b){return a-b;}, 2, '-');

} // namespace Tokens
} // namespace Pable
