#pragma once

#include "Token/token.h"
#include <memory>

namespace Pable {
namespace Tokens {

class Operator : public Token
{
public:
    Operator(int precedence, char symbol);
    Operator(const Operator& other);

    int precedence() const;
    char symbol() const;

    friend void swap(Operator& first, Operator& second);
    Operator &operator=(Operator other);
    bool operator==(const Operator &other) const;

private:
    int mPrecedence;
    char mSymbol;
};

extern std::shared_ptr<Operator> plusOp;
extern std::shared_ptr<Operator> minusOp;

} // namespace Tokens
} // namespace Pable
