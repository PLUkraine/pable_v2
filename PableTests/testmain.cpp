#include <QtTest>

#include "parsertest.h"
#include "dirtygraphtest.h"


int main(int argc, char** argv)
{
   int status = 0;
   auto ASSERT_TEST = [&status, argc, argv](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
        delete obj;
   };

   ASSERT_TEST(new ParserTest());
   ASSERT_TEST(new DirtyGraphTest());

   return status;
}
