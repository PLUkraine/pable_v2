#include "parsertest.h"
#include "token.h"
#include "testutils.h"


void ParserTest::testTokenizer()
{
    Pable::Tokenizer t;
    auto res = t.tokenize("3+4");
    QList<QString> expected;
    expected << "3" << "+" << "4";
    compareQLists(res, expected);

    expected.clear();
    res = t.tokenize("133 -\t4214");
    expected << "133" << "-" << "4214";
    compareQLists(expected, res);

    expected.clear();
    res = t.tokenize("-00123");
    expected << "-" << "123";
    compareQLists(expected, res);

    expected.clear();
    QVERIFY_EXCEPTION_THROWN(t.tokenize("2+surprise"), std::runtime_error);

}
