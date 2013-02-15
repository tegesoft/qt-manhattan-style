cmake_minimum_required(VERSION 2.8.9)

project(qt-manhattan-style)

set(SRCS
    stylehelper.cpp
    styledbar.cpp
    styleanimator.cpp
    stringutils.cpp
    qtcolorbutton.cpp
    qtcassert.cpp
    progressbar.cpp
    minisplitter.cpp
    manhattanstyle.cpp
    historycompleter.cpp
    fancytabwidget.cpp
    fancymainwindow.cpp
    fancylineedit.cpp
    fancyactionbar.cpp
    doubletabwidget.cpp
    extensions/simpleprogressbar.cpp
    stylehelper.h
    styledbar.h
    styleanimator.h
    stringutils.h
    qtcolorbutton.h
    qtcolorbutton_p.h
    qtcassert.h
    progressbar.h
    minisplitter.h
    manhattanstyle.h
    historycompleter.h
    fancytabwidget.h
    fancymainwindow.h
    fancylineedit.h
    fancyactionbar.h
    doubletabwidget.h
    coreconstants.h
    qt-manhattan-style_global.hpp
    extensions/simpleprogressbar.h
    extensions/tabwidget.h
    extensions/tabwidget.cpp
    extensions/threelevelsitempicker.h
    extensions/threelevelsitempicker.cpp
)

set(RCS
    resources/resources.qrc
)

set(UI_FILES
    doubletabwidget.ui
)

# Qt5
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)

# Run uic on ui files
qt5_wrap_ui(UI_HDRS ${UI_FILES})

# this command will generate rules that will run rcc on all files from RCS
# in result RC_SRCS variable will contain paths to files produced by rcc
qt5_add_resources(RC_SRCS ${RCS})

# The code still used some deprecated stuff
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x040900)

# Create the target
add_definitions(-DQTMANHATTANSTYLE_LIBRARY)
add_library(${PROJECT_NAME} SHARED ${SRCS} ${UI_HDRS} ${RC_SRCS})

# Link the target to its dependencies
# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES})

qt5_use_modules(${PROJECT_NAME} Widgets)
