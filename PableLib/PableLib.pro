TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    cellindex.cpp \
    dirtygraph.cpp \
    expressioncontext.cpp \
    parser.cpp
HEADERS += \
    cellindex.h \
    dirtygraph.h \
    expressioncontext.h \
    parser.h
