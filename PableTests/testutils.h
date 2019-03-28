#pragma once

#include <QList>
#include <memory>

#include "Token/token.h"


void compareTokenLists(const QList<std::shared_ptr<Pable::Tokens::Token>> &l1,
                       const QList<std::shared_ptr<Pable::Tokens::Token>> &l2);
