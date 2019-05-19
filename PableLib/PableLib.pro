TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    cellindex.cpp \
    dirtygraph.cpp \
    expressioncontext.cpp \
    parser.cpp \
    spreadsheetgraph.cpp
HEADERS += \
    cellindex.h \
    dirtygraph.h \
    expressioncontext.h \
    parser.h \
    spreadsheetgraph.h
