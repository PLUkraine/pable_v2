#include "token.h"
#include <exception>

namespace Pable {
namespace Tokens {

Token::Type Token::type() const
{
    return mType;
}

Token::Token(Token::Type type)
    : mType(type)
{
}

} // namespace Tokens
} // namespace Pable
