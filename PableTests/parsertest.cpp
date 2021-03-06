#include "parsertest.h"
#include "parser.h"
#include "testutils.h"


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
    MapExpressionContext context;
    context.map = {
        {*CellIndex::str("$A1"), 1},
        {*CellIndex::str("$BB3"), 10},
    };

    Expression expr;
    expr.evaluate(context);
    std::optional<int> actual = expr.result();
    std::optional<int> expected = 0;
    QCOMPARE(actual, expected);

    expr.setExpression({2, 3, '-', 6, 7, '+', '-'});
    QVERIFY_EXCEPTION_THROWN(expr.result(), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(expr.error(), std::runtime_error);
    expr.evaluate(context);
    actual = expr.result();
    expected = -14;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::nullopt);

    expr.setExpression({4, 5, '+', 1, '-', *CellIndex::str("$BB3"), '+'});
    expr.evaluate(context);
    actual = expr.result();
    expected = 18;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::nullopt);

    expr.setExpression({4});
    expr.evaluate(context);
    actual = expr.result();
    expected = 4;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::nullopt);

    expr.setExpression({9, 2});
    expr.evaluate(context);
    actual = expr.result();
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::optional<Expression::Error>(Expression::BadExpression));

    expr.setExpression({});
    expr.evaluate(context);
    actual = expr.result();
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::optional<Expression::Error>(Expression::BadExpression));

    expr.setExpression({9, '+', '-'});
    expr.evaluate(context);
    actual = expr.result();
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::optional<Expression::Error>(Expression::BadExpression));

    expr.setExpression({9, 2, '$'});
    expr.evaluate(context);
    actual = expr.result();
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::optional<Expression::Error>(Expression::BadExpression));

    expr.setExpression({9, *CellIndex::str("$AA2"), '-'});
    expr.evaluate(context);
    actual = expr.result();
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::optional<Expression::Error>(Expression::BadExpression));

    expr.setExpression({9, 6, '-', '-'});
    expr.evaluate(context);
    actual = expr.result();
    expected = std::nullopt;
    QCOMPARE(actual, expected);
    QCOMPARE(expr.error(), std::optional<Expression::Error>(Expression::BadExpression));
}

void ParserTest::testGetResult()
{
    MapExpressionContext context;
    context.map = {
        {*CellIndex::str("$A1"), 1},
        {*CellIndex::str("$BB3"), 10},
    };

    Expression expr;
    std::optional<int> actual = expr.result();
    std::optional<int> expected = 0;
    QCOMPARE(actual, expected);

    expr.setExpression({2, 3, '-', 6, 7, '+', '-'});
    QVERIFY_EXCEPTION_THROWN(expr.result(), std::runtime_error);

    expr.evaluate(context);
    actual = expr.result();
    expected = -14;
    QCOMPARE(actual, expected);
    expr.evaluate(context);
    actual = expr.result();
    expected = -14;
    QCOMPARE(actual, expected);

    expr.setExpression({4, 5, '+', 1, '-', *CellIndex::str("$BB3"), '+'});
    QVERIFY_EXCEPTION_THROWN(expr.result(), std::runtime_error);

    expr.evaluate(context);
    actual = expr.result();
    expected = 18;
    QCOMPARE(actual, expected);
    actual = expr.result();
    expected = 18;
    QCOMPARE(actual, expected);

    // reval with different context
    context.map[*CellIndex::str("$BB3")] = -1;
    expr.evaluate(context);
    actual = expr.result();
    expected = 7;
    QCOMPARE(actual, expected);
    expr.evaluate(context);
    actual = expr.result();
    expected = 7;
    QCOMPARE(actual, expected);
    expr.evaluate(context);
    actual = expr.result();
    expected = 7;
    QCOMPARE(actual, expected);
}

void ParserTest::testCopying()
{
    const ExpressionContext &context = *NullExpressionContext::get();

    Expression e1;
    e1.setExpression({10, 5, '+'});

    Expression e2(e1);
    QCOMPARE(e1.wasEvaluated(), e2.wasEvaluated());
    QCOMPARE(e1.toString(), e2.toString());
    e1.evaluate(context);
    QVERIFY(e1.wasEvaluated() != e2.wasEvaluated());
    e2.evaluate(context);
    QCOMPARE(e1.wasEvaluated(), e2.wasEvaluated());
    QCOMPARE(e1.result(), e2.result());

    e1.setExpression({9});
    e1.evaluate(context);
    QCOMPARE(e1.result(), 9);
    QCOMPARE(e2.result(), 15);
}

void ParserTest::testToString()
{
    Expression expr;
    auto actual = expr.toString();
    std::string expected = "0";

    expr.setExpression({4, 5, '+', 1, CellIndex(0, 1)});
    actual = expr.toString();
    expected = "4 5 + 1 $B0";
    QCOMPARE(actual, expected);

    expr.setExpression({*CellIndex::str("$DXH100"), '+'});
    actual = expr.toString();
    expected = "$DXH100 +";
    QCOMPARE(actual, expected);

    expr.setExpression({});
    actual = expr.toString();
    expected = "";
    QCOMPARE(actual, expected);

    expr.setExpression({1});
    actual = expr.toString();
    expected = "1";
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

    actual = tokenizer.tokenize("( -4 +5 ) -");
    expected = {'(', -4, 5, ')', '-'};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("-4 +5 $A1 -");
    expected = {-4, 5, *CellIndex::str("$A1"), '-',};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize("+ -100002 - $BBAS1213");
    expected = {'+', -100002, '-', *CellIndex::str("$BBAS1213")};
    QCOMPARE(actual, expected);

    actual = tokenizer.tokenize(") ( + -100002 ) - $BBAS1213");
    expected = {')', '(', '+', -100002, ')', '-', *CellIndex::str("$BBAS1213")};
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

void ParserTest::testParser()
{
    ShuntingYardParser parser;

    auto actual = parser.convertToRpn({0});
    std::vector<Token> expected = {0};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({1, '+', 2});
    expected = {1, 2, '+'};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({-3, '-', 2, '+', 12});
    expected = {-3, 2, '-', 12, '+'};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({-3, '-', '(', 2, '+', 12, ')'});
    expected = {-3, 2, 12, '+', '-'};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({4, '-', '(', 2, '-', '(', 5, '+', 6, '+', 1, ')', '+', -1, ')'});
//        qDebug() << QString::fromStdString(Expression::toString(actual));
    expected = {4, 2, 5, 6, '+', 1, '+', '-', -1, '+', '-'};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({'(', 2, ')', '+', 10});
    expected = {2, 10, '+'};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({'(', 2, '+', 4, ')', '+', 10});
    expected = {2, 4, '+', 10, '+'};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({'(', 2});
    expected = {};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({2, ')'});
    expected = {};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({2, '('});
    expected = {};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({')', 2});
    expected = {};
    QCOMPARE(actual, expected);

    actual = parser.convertToRpn({'(', 2, '+', '(', 5, '-', 6, ')'});
    expected = {};
    QCOMPARE(actual, expected);
}
