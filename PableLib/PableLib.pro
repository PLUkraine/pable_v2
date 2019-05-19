TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    cellindex.cpp \
    expressioncontext.cpp \
    parser.cpp \
    spreadsheetgraph.cpp
HEADERS += \
    cellindex.h \
    expressioncontext.h \
    parser.h \
    spreadsheetgraph.h
