#pragma once

#include "Token/token.h"
#include <memory>

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

std::shared_ptr<DoubleLiteral> doubleLiteral(double value);

} // namespace Tokens
} // namespace Pable
