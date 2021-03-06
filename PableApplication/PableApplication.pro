QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PableApplication
TEMPLATE = app

include(../common.pri)

INCLUDEPATH += ./
SOURCES += \
        mainwindow.cpp\
        main.cpp \
    spreadsheetmodel.cpp

HEADERS += \
        mainwindow.h \
    spreadsheetmodel.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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
