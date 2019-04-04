#include "testutils.h"
#include <QtTest>
#include <QString>

#include "Token/doubleliteral.h"
#include "Token/operator.h"

void compareTokenLists(const QList<std::shared_ptr<Pable::Tokens::Token>> &l1,
                       const QList<std::shared_ptr<Pable::Tokens::Token>> &l2)
{
    QCOMPARE(l1.size(), l2.size());

    for (int i=0; i<l1.size(); ++i) {
        QCOMPARE(l1[0]->type(), l2[0]->type());

        if (l1[i]->type() == Pable::Tokens::Token::Literal)
        {
            auto t1 = std::dynamic_pointer_cast<Pable::Tokens::DoubleLiteral>(l1[i]);
            auto t2 = std::dynamic_pointer_cast<Pable::Tokens::DoubleLiteral>(l2[i]);
            QVERIFY2(*t1 == *t2, QString("%1 != %2").arg(t1->value()).arg(t2->value()).toStdString().c_str());
        }
        if (l1[i]->type() == Pable::Tokens::Token::Operator)
        {
            auto t1 = std::dynamic_pointer_cast<Pable::Tokens::Operator>(l1[i]);
            auto t2 = std::dynamic_pointer_cast<Pable::Tokens::Operator>(l2[i]);
            QVERIFY2(*t1 == *t2, QString("%1 != %2").arg(t1->symbol()).arg(t2->symbol()).toStdString().c_str());
        }
    }
}
