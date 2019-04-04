TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    cellindex.cpp \
    Token/token.cpp \
    tokenizer.cpp \
    Token/operator.cpp \
    Token/doubleliteral.cpp
HEADERS += \
    cellindex.h \
    Token/token.h \
    tokenizer.h \
    Token/operator.h \
    Token/doubleliteral.h
