#pragma once

#include <QtTest>

class SpreadsheetGraphTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSetExpressionWithoutUpdateSimple();
    void testUpdateExpressionSimple();
    void testUpdateExpressionWithDependencies();
};
