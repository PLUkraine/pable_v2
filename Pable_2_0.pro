TEMPLATE = subdirs

SUBDIRS += \
    PableApplication\
    GraphLib

# CONFIG += ordered

PableApplication.file = PableApplication/PableApplication.pro
GraphLib.file = GraphLib/GraphLib.pro

# what subproject depends on others
PableApplication.depends = GraphLib
