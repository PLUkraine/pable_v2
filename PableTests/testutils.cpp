#include "testutils.h"
#include <QtTest>
#include <QString>

int fuzzyCmp(float a, float b, const float EPS)
{
    const float dif = a-b;
    if (-EPS < dif && dif < EPS) return 0;
    else if (dif < 0) return -1;
    else return 1;
}

QDebug operator<<(QDebug d, const std::optional<int> &v) {
    static QString NULLOPT = "N";
    d << (v.has_value() ? QString::number(*v) : NULLOPT);
    return d;
}
