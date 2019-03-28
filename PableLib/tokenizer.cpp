#include "tokenizer.h"

#include "Token/literal.h"
#include "Token/operator.h"


namespace Pable {

enum class TokenizerState {
    Number, Whitespace
};

QList<std::shared_ptr<Tokens::Token> > Tokenizer::tokenize(const QString &exp) const
{
    int numSoFar = 0;
    TokenizerState state = TokenizerState::Whitespace;
    QList<std::shared_ptr<Tokens::Token> > answer;

    for (int i=0; i<exp.length(); ++i) {
        QChar c = exp[i];
        if (c.isDigit()) {
            state = TokenizerState::Number;
            int d = c.toLatin1() - '0';
            numSoFar = numSoFar * 10 + d;
        }
        else {
            if (state == TokenizerState::Number) {
                answer << std::make_shared<Tokens::Literal>(numSoFar);
                numSoFar = 0;
                state = TokenizerState::Whitespace;
            }

            if (isOp(c.toLatin1())) {
                auto op = c == '+' ? Tokens::plusOp : Tokens::minusOp;
                answer << std::make_shared<Tokens::Operator>(op);
            }
            else if (!c.isSpace()){
                QString message = "Unknown char: %1";
                throw std::runtime_error(message.arg(c).toStdString());
            }
        }
    }

    if (state == TokenizerState::Number) {
        answer << std::make_shared<Tokens::Literal>(numSoFar);
    }

    return answer;
}

bool Tokenizer::isOp(char c) const
{
    return c == '+' || c == '-';
}

} // namespace Pable
