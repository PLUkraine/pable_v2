TEMPLATE = lib
CONFIG += staticlib

include(../common.pri)

# Input
SOURCES += \
    expression.cpp \
    graph.cpp \
    graphloopdetector.cpp \
    graphnode.cpp
HEADERS += \
    expression.h \
    graph.h \
    graphloopdetector.h \
    graphnode.h \
    utils.h
