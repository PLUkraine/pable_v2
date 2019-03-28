#pragma once

namespace Pable {
namespace Tokens {

class Token
{
public:
    enum Type
    {
        Operator, Literal, CellIndex
    };

    virtual ~Token() {}
    Type type() const;

protected:
    Token(Type type);

private:
    Type mType;
};

} // namespace Tokens
} // namespace Pable
