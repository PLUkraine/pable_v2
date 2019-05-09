#include "parsertest.h"
#include "testutils.h"


void ParserTest::testTokenizer()
{
    QCOMPARE(fuzzyCmp(0.1f, 0.4f), -1);
    QCOMPARE(fuzzyCmp(0.1f, -0.4f), 1);
    QCOMPARE(fuzzyCmp(0.1f, 0.100000000001f), 0);
//    QVERIFY_EXCEPTION_THROWN(t.tokenize("-a-b-"), std::runtime_error);

}
