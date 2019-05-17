#pragma once

#include <QtTest>

class DirtyGraphTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testClear();
    void testCycleDetection();
    void testInvalidate();
};
