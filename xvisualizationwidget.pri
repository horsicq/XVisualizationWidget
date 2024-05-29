INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/dialogvisualization.h \
    $$PWD/dialogvisualizationprocess.h \
    $$PWD/xfilearrow.h \
    $$PWD/xfiledescription.h \
    $$PWD/xfileimage.h \
    $$PWD/xvisualization.h \
    $$PWD/xvisualizationimage.h \
    $$PWD/xvisualizationwidget.h

SOURCES += \
    $$PWD/dialogvisualization.cpp \
    $$PWD/dialogvisualizationprocess.cpp \
    $$PWD/xfilearrow.cpp \
    $$PWD/xfiledescription.cpp \
    $$PWD/xfileimage.cpp \
    $$PWD/xvisualization.cpp \
    $$PWD/xvisualizationimage.cpp \
    $$PWD/xvisualizationwidget.cpp

FORMS += \
    $$PWD/dialogvisualization.ui \
    $$PWD/xvisualizationwidget.ui

DISTFILES += \
    $$PWD/LICENSE \
    $$PWD/README.md \
    $$PWD/xvisualizationwidget.cmake
