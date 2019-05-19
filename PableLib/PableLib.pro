TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    cellindex.cpp \
    dirtygraph.cpp \
    parser.cpp
HEADERS += \
    cellindex.h \
    dirtygraph.h \
    parser.h
