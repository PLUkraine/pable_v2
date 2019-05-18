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
    QCOMPARE(CellIndex::str("$A0"), CellIndex(0, 0));
    QCOMPARE(CellIndex::str("$G20"), CellIndex(20, 'G'-'A'));
    QCOMPARE(CellIndex::str("$EX10"), CellIndex(10, 153));
    QCOMPARE(CellIndex::str("~$EX10"),      std::nullopt);
    QCOMPARE(CellIndex::str("$EX10$EX1"),   std::nullopt);
    QCOMPARE(CellIndex::str("$AAA"),        std::nullopt);
    QCOMPARE(CellIndex::str("$30"),         std::nullopt);
    QCOMPARE(CellIndex::str("$ZA#30"),      std::nullopt);
    QCOMPARE(CellIndex::str("$ZA30 "),      std::nullopt);
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

void ParserTest::testEvaluate()
{
    std::unordered_map<CellIndex, std::optional<int>> cellValues = {
        {*CellIndex::str("$A1"), 1},
        {*CellIndex::str("$BB3"), 10},
    };

    Expression expr;
    std::optional<int> actual = expr.evaluate(cellValues);
    std::optional<int> expected = 0;
    QCOMPARE(actual, expected);

    expr.setExpression({2, 3, '-', 6, 7, '+', '-'});
    actual = expr.evaluate(cellValues);
    expected = -14;
    QCOMPARE(actual, expected);

    expr.setExpression({4, 5, '+', 1, '-', *CellIndex::str("$BB3"), '+'});
    actual = expr.evaluate(cellValues);
    expected = 18;
    QCOMPARE(actual, expected);

    expr.setExpression({4});
    actual = expr.evaluate(cellValues);
    expected = 4;
    QCOMPARE(actual, expected);

    expr.setExpression({9, 2});
    actual = expr.evaluate(cellValues);
    expected = std::nullopt;
    QCOMPARE(actual, expected);

    expr.setExpression({});
    actual = expr.evaluate(cellValues);
    expected = std::nullopt;
    QCOMPARE(actual, expected);

    expr.setExpression({9, '+', '-'});
    actual = expr.evaluate(cellValues);
    expected = std::nullopt;
    QCOMPARE(actual, expected);

    expr.setExpression({9, 2, '$'});
    actual = expr.evaluate(cellValues);
    expected = std::nullopt;
    QCOMPARE(actual, expected);

    expr.setExpression({9, *CellIndex::str("$AA2"), '-'});
    actual = expr.evaluate(cellValues);
    expected = std::nullopt;
    QCOMPARE(actual, expected);

    expr.setExpression({9, 6, '-', '-'});
    actual = expr.evaluate(cellValues);
    expected = std::nullopt;
    QCOMPARE(actual, expected);
}

void ParserTest::testTokenizer()
{
    Tokenizer tokenizer;

    auto actual = tokenizer.tokenize("4 5 +");
    std::vector<Token> expected = {4, 5, '+'};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("-4 +5 -");
    expected = {-4, 5, '-'};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("-4 +5 $A1 -");
    expected = {-4, 5, *CellIndex::str("$A1"), '-',};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("+ -100002 - $BBAS1213");
    expected = {'+', -100002, '-', *CellIndex::str("$BBAS1213")};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("$ B 12 ");
    expected = {};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("3 * 4");
    expected = {};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("$1A");
    expected = {};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("213.4");
    expected = {};
    QCOMPARE(actual, expected);
}
