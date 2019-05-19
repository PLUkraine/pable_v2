#pragma once

#include <QtTest>

class ParserTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testAlphaToIndex();
    void testCellIndexParse();
    void testGetDependencies();
    void testEvaluate();
    void testGetResult();
    void testCopying();
    void testToString();

    void testTokenizer();
};
