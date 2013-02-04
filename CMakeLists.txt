cmake_minimum_required(VERSION 2.8)

project(qt-manhattan-style)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRCS
    src/stylehelper.cpp
    src/styledbar.cpp
    src/styleanimator.cpp
    src/stringutils.cpp
    src/qtcolorbutton.cpp
    src/qtcassert.cpp
    src/progressbar.cpp
    src/minisplitter.cpp
    src/manhattanstyle.cpp
    src/historycompleter.cpp
    src/fancytabwidget.cpp
    src/fancymainwindow.cpp
    src/fancylineedit.cpp
    src/fancyactionbar.cpp
    src/doubletabwidget.cpp
    src/extensions/simpleprogressbar.cpp
    src/stylehelper.h
    src/styledbar.h
    src/styleanimator.h
    src/stringutils.h
    src/qtcolorbutton.h
    src/qtcolorbutton_p.h
    src/qtcassert.h
    src/progressbar.h
    src/minisplitter.h
    src/manhattanstyle.h
    src/historycompleter.h
    src/fancytabwidget.h
    src/fancymainwindow.h
    src/fancylineedit.h
    src/fancyactionbar.h
    src/doubletabwidget.h
    src/coreconstants.h
    src/qt-manhattan-style_global.hpp
    src/extensions/simpleprogressbar.h
)

set(RCS
    src/resources/resources.qrc
)

set(UI_FILES
    src/doubletabwidget.ui
)

# Qt5
find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)

# Run uic on ui files
qt5_wrap_ui(UI_HDRS ${UI_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # UI headers are generated in the binary directory

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
