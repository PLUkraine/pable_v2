#include "parsertest.h"
#include "testutils.h"

#include "tokenizer.h"
#include "Token/doubleliteral.h"
#include "Token/operator.h"


using TokenType = Pable::Tokens::Token;
using Pable::Tokens::DoubleLiteral;
using Pable::Tokens::Operator;


void ParserTest::testTokenizer()
{
    Pable::Tokenizer t;
    auto res = t.tokenize("3+4");
    QList<std::shared_ptr<Pable::Tokens::Token>> expected;
    expected << std::make_shared<DoubleLiteral>(3.0)
             << Pable::Tokens::plusOp
             << std::make_shared<DoubleLiteral>(4.0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("\t -  402 +142+1 ");
    expected << Pable::Tokens::minusOp
             << std::make_shared<DoubleLiteral>(402)
             << Pable::Tokens::plusOp
             << std::make_shared<DoubleLiteral>(142)
             << Pable::Tokens::plusOp
             << std::make_shared<DoubleLiteral>(1.0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("21830192");
    expected << std::make_shared<DoubleLiteral>(21830192);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("+001232");
    expected << Pable::Tokens::plusOp
             << std::make_shared<DoubleLiteral>(1232);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("0");
    expected << std::make_shared<DoubleLiteral>(0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("3-5");
    expected << std::make_shared<DoubleLiteral>(3)
             << Pable::Tokens::minusOp
             << std::make_shared<DoubleLiteral>(5);
    compareTokenLists(res, expected);

    QVERIFY_EXCEPTION_THROWN(t.tokenize("2+surprise"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(t.tokenize("$+12"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(t.tokenize("-a-b-"), std::runtime_error);

}
