#pragma once

#include "Token/token.h"

namespace Pable {
namespace Tokens {

class DoubleLiteral : public Token
{
public:
    DoubleLiteral(double value);

    double value() const;
    bool operator==(const DoubleLiteral &other) const;

private:
    double mValue;
};

} // namespace Tokens
} // namespace Pable
