#pragma once

#include <QString>
#include <QList>

#include <memory>

#include "Token/token.h"

namespace Pable {

class Tokenizer
{
public:
    QList<std::shared_ptr<Tokens::Token>> tokenize(const QString &exp) const;

    bool isOp(char c) const;
};

} // namespace Pable
