#include "testutils.h"
#include <QtTest>
#include <QString>


void compareQLists(const QList<QString> &l1, const QList<QString> &l2)
{
    QCOMPARE(l1.size(), l2.size());

    for (int i=0; i<l1.size(); ++i) {
        QCOMPARE(l1[i], l2[i]);
    }
}
