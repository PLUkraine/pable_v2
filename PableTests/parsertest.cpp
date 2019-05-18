#include "parsertest.h"
#include "parser.h"
#include "testutils.h"

QDebug operator<<(QDebug d, const CellIndex &v) {
    d << v.row() << ";" << v.col();
    return d;
}


void ParserTest::testAlphaToIndex()
{
    QCOMPARE(CellIndex::columnAlphaToIndex("A"), 0);
    QCOMPARE(CellIndex::columnAlphaToIndex("AA"), 26);
    QCOMPARE(CellIndex::columnAlphaToIndex("AAA"), 26*26+26);
    QCOMPARE(CellIndex::columnAlphaToIndex("BC"), 2*26+2);
    QCOMPARE(CellIndex::columnAlphaToIndex("ZZ"), 26*26+25);
    QVERIFY_EXCEPTION_THROWN(CellIndex::columnAlphaToIndex("BA1"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex::columnAlphaToIndex("B~"), std::runtime_error);
}

void ParserTest::testCellIndexParse()
{
    QCOMPARE(CellIndex("$A0"), CellIndex(0, 0));
    QCOMPARE(CellIndex("$G20"), CellIndex(20, 'G'-'A'));
    QCOMPARE(CellIndex("$EX10"), CellIndex(10, 153));
    QVERIFY_EXCEPTION_THROWN(CellIndex("~$EX10"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("$EX10$EX1"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("$AAA"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("$30"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("$ZA#30"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("$ZA30 "), std::runtime_error);
}

void ParserTest::testGetDependencies()
{
    Expression expr;
    std::vector<CellIndex> actual = expr.dependencies();
    std::vector<CellIndex> expected = {};
    QCOMPARE(actual, expected);

    expr.setExpression({4, 5, '+', 1, CellIndex(0, 1)});
    actual = expr.dependencies();
    expected = {CellIndex(0, 1)};
    QCOMPARE(actual, expected);

    expr.setExpression({CellIndex(25, 1), CellIndex(31, 45), '+', 1, CellIndex(0, 1)});
    actual = expr.dependencies();
    expected = {CellIndex(25, 1), CellIndex(31, 45), CellIndex(0, 1)};
    QCOMPARE(actual, expected);

    expr.setExpression({2, 3, '-', 6, 7, '+', '-'});
    actual = expr.dependencies();
    expected = {};
    QCOMPARE(actual, expected);
}
