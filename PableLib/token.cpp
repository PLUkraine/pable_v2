#include "token.h"
#include <exception>

namespace Pable {

enum class TokenizerState {
    Number, Whitespace
};

QList<QString> Tokenizer::tokenize(const QString &exp) const
{
    int numSoFar = 0;
    TokenizerState state = TokenizerState::Whitespace;
    QList<QString> answer;

    for (int i=0; i<exp.length(); ++i) {
        QChar c = exp[i];
        if (c.isDigit()) {
            state = TokenizerState::Number;
            int d = c.toLatin1() - '0';
            numSoFar = numSoFar * 10 + d;
        }
        else {
            if (state == TokenizerState::Number) {
                answer << QString::number(numSoFar);
                numSoFar = 0;
                state = TokenizerState::Whitespace;
            }

            if (isOp(c.toLatin1())) {
                answer << QString(c);
            }
            else if (!c.isSpace()){
                QString message = "Unknown char: %1";
                throw std::runtime_error(message.arg(c).toStdString());
            }
        }
    }

    if (state == TokenizerState::Number) {
        answer << QString::number(numSoFar);
    }

    return answer;
}

bool Tokenizer::isOp(char c) const
{
    return c == '+' || c == '-';
}

}
