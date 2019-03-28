#include "parsertest.h"
#include "testutils.h"

#include "tokenizer.h"
#include "Token/literal.h"
#include "Token/operator.h"


using TokenType = Pable::Tokens::Token;
using Pable::Tokens::Literal;
using Pable::Tokens::Operator;


void ParserTest::testTokenizer()
{
    Pable::Tokenizer t;
    auto res = t.tokenize("3+4");
    QList<std::shared_ptr<Pable::Tokens::Token>> expected;
    expected << std::make_shared<Literal>(3.0)
             << std::make_shared<Operator>(Pable::Tokens::plusOp)
             << std::make_shared<Literal>(4.0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("\t -  402 +142+1 ");
    expected << std::make_shared<Operator>(Pable::Tokens::minusOp)
             << std::make_shared<Literal>(402)
             << std::make_shared<Operator>(Pable::Tokens::plusOp)
             << std::make_shared<Literal>(142)
             << std::make_shared<Operator>(Pable::Tokens::plusOp)
             << std::make_shared<Literal>(1.0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("21830192");
    expected << std::make_shared<Literal>(21830192);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("+001232");
    expected << std::make_shared<Operator>(Pable::Tokens::plusOp)
             << std::make_shared<Literal>(1232);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("0");
    expected << std::make_shared<Literal>(0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("3-5");
    expected << std::make_shared<Literal>(3)
             << std::make_shared<Operator>(Pable::Tokens::minusOp)
             << std::make_shared<Literal>(5);
    compareTokenLists(res, expected);

    QVERIFY_EXCEPTION_THROWN(t.tokenize("2+surprise"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(t.tokenize("$+12"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(t.tokenize("-a-b-"), std::runtime_error);

}
