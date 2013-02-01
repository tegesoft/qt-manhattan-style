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
)

set(MOC_HDRS
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

# Find external libraries
find_package(Qt4 4.8.0 COMPONENTS QtCore QtGui REQUIRED)

# (QT_USE_FILE is a variable defined by find_package(Qt4) that contains a path to CMake script)
include(${QT_USE_FILE})

# Run moc on needed files
set(MOC_SRCS) # clear the variable as the next command doesn't do it
QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})
source_group(moc FILES ${MOC_SRCS})

# Run uic on ui files
QT4_WRAP_UI(UI_HDRS ${UI_FILES})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}) # UI headers are generated in the binary directory

# this command will generate rules that will run rcc on all files from RCS
# in result RC_SRCS variable will contain paths to files produced by rcc
QT4_ADD_RESOURCES(RC_SRCS ${RCS})

# Create the target
add_definitions("-DQTMANHATTANSTYLE_LIBRARY")
add_library(${PROJECT_NAME} SHARED ${MOC_HDRS} ${UI_HDRS} ${SRCS} ${MOC_SRCS} ${RC_SRCS})

# Link the target to its dependencies
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})
