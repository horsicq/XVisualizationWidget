INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/xvisualizationwidget.ui

HEADERS += \
    $$PWD/dialogvisualizationprocess.h \
    $$PWD/xfilearrow.h \
    $$PWD/xfiledescription.h \
    $$PWD/xfileimage.h \
    $$PWD/xvisualization.h \
    $$PWD/xvisualizationwidget.h

SOURCES += \
    $$PWD/dialogvisualizationprocess.cpp \
    $$PWD/xfilearrow.cpp \
    $$PWD/xfiledescription.cpp \
    $$PWD/xfileimage.cpp \
    $$PWD/xvisualization.cpp \
    $$PWD/xvisualizationwidget.cpp

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xvisualizationwidget.cmake
