#-------------------------------------------------
#
# Project created by QtCreator 2013-01-31T11:37:22
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The code still used some deprecated stuff
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x040900

TARGET = qt-manhattan-style
TEMPLATE = lib

DEFINES += QTMANHATTANSTYLE_LIBRARY

INCLUDEPATH += src

SOURCES += \
    stylehelper.cpp \
    styledbar.cpp \
    styleanimator.cpp \
    stringutils.cpp \
    qtcolorbutton.cpp \
    qtcassert.cpp \
    progressbar.cpp \
    minisplitter.cpp \
    manhattanstyle.cpp \
    historycompleter.cpp \
    fancytabwidget.cpp \
    fancymainwindow.cpp \
    fancylineedit.cpp \
    fancyactionbar.cpp \
    doubletabwidget.cpp \
    extensions/simpleprogressbar.cpp \
    extensions/tabwidget.cpp \
    extensions/threelevelsitempicker.cpp

HEADERS +=\
    stylehelper.h \
    styledbar.h \
    styleanimator.h \
    stringutils.h \
    qtcolorbutton.h \
    qtcolorbutton_p.h \
    qtcassert.h \
    progressbar.h \
    minisplitter.h \
    manhattanstyle.h \
    historycompleter.h \
    fancytabwidget.h \
    fancymainwindow.h \
    fancylineedit.h \
    fancyactionbar.h \
    doubletabwidget.h \
    coreconstants.h \
    qt-manhattan-style_global.hpp \
    extensions/simpleprogressbar.h \
    extensions/tabwidget.h \
    extensions/threelevelsitempicker.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    doubletabwidget.ui

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    CMakeLists.txt
