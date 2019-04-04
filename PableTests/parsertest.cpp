#include "parsertest.h"
#include "testutils.h"

#include "tokenizer.h"
#include "Token/doubleliteral.h"
#include "Token/operator.h"

using namespace Pable::Tokens;


void ParserTest::testTokenizer()
{
    Pable::Tokenizer t;
    auto res = t.tokenize("3+4");
    QList<std::shared_ptr<Token>> expected;
    expected << doubleLiteral(3.0)
             << plusOp
             << doubleLiteral(4.0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("\t -  402 +142+1 ");
    expected << minusOp
             << doubleLiteral(402)
             << plusOp
             << doubleLiteral(142)
             << plusOp
             << doubleLiteral(1.0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("21830192");
    expected << doubleLiteral(21830192);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("+001232");
    expected << plusOp
             << doubleLiteral(1232);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("0");
    expected << doubleLiteral(0);
    compareTokenLists(res, expected);

    expected.clear();
    res = t.tokenize("3-5");
    expected << doubleLiteral(3)
             << minusOp
             << doubleLiteral(5);
    compareTokenLists(res, expected);

    QVERIFY_EXCEPTION_THROWN(t.tokenize("2+surprise"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(t.tokenize("$+12"), std::runtime_error);
    QVERIFY_EXCEPTION_THROWN(t.tokenize("-a-b-"), std::runtime_error);

}
