TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    dirtygraph.cpp \
    parser.cpp
HEADERS += \
    dirtygraph.h \
    parser.h
