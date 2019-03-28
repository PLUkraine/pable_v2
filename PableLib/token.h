#pragma once

#include <QString>
#include <QList>

namespace Pable {

class Tokenizer
{
public:
    QList<QString> tokenize(const QString &exp) const;

    bool isOp(char c) const;
};

}
