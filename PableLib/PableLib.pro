TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    cellindex.cpp \
    Token/token.cpp \
    tokenizer.cpp \
    Token/literal.cpp \
    Token/operator.cpp
HEADERS += \
    cellindex.h \
    Token/token.h \
    tokenizer.h \
    Token/operator.h \
    Token/doubleliteral.h
