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
    QVERIFY(CellIndex("A0") == CellIndex(0, 0));
    QVERIFY(CellIndex("G20") == CellIndex(20, 'G'-'A'));
    QVERIFY(CellIndex("EX10") == CellIndex(10, 153));
    QVERIFY_EXCEPTION_THROWN(CellIndex("~EX10"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("EX10EX1"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("AAA"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("30"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("ZA#30"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(CellIndex("ZA30 "), std::runtime_error);
}
