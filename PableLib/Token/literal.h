#pragma once

#include "Token/token.h"

namespace Pable {
namespace Tokens {

class Literal : public Token
{
public:
    Literal(double value);

    double value() const;
    bool operator==(const Literal &other) const;

private:
    double mValue;
};

} // namespace Tokens
} // namespace Pable
