QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

include(../common.pri)

TEMPLATE = app

SOURCES += \ 
    testmain.cpp \
    parsertest.cpp \
    testutils.cpp

# PableLib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PableLib/release/ -lPableLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PableLib/debug/ -lPableLib
else:unix: LIBS += -L$$OUT_PWD/../PableLib/ -lPableLib
INCLUDEPATH += $$PWD/../PableLib
DEPENDPATH += $$PWD/../PableLib
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PableLib/release/libPableLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PableLib/debug/libPableLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PableLib/release/PableLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../PableLib/debug/PableLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../PableLib/libPableLib.a

HEADERS += \
    parsertest.h \
    testutils.h
