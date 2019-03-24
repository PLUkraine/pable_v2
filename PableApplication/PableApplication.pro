QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PableApplication
TEMPLATE = app

include(../common.pri)

INCLUDEPATH += ./
SOURCES += \
        mainwindow.cpp\
        main.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# GraghLib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GraphLib/release/ -lGraphLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GraphLib/debug/ -lGraphLib
else:unix: LIBS += -L$$OUT_PWD/../GraphLib/ -lGraphLib
INCLUDEPATH += $$PWD/../GraphLib
DEPENDPATH += $$PWD/../GraphLib
win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GraphLib/release/libGraphLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GraphLib/debug/libGraphLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GraphLib/release/GraphLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GraphLib/debug/GraphLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../GraphLib/libGraphLib.a
