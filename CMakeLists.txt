cmake_minimum_required( VERSION 3.4 )
project( MccPinewood )

FIND_PACKAGE( Qt5Widgets )
FIND_PACKAGE( Qt5Gui )
FIND_PACKAGE( Qt5PrintSupport )
FIND_PACKAGE( OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer osgText )

INCLUDE_DIRECTORIES( ${OPENSCENEGRAPH_INCLUDE_DIRS} )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED 1 )
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_AUTOUIC ON )
SET( CMAKE_AUTORCC ON )

IF (NOT TARGET gtest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(libs/googletest)
ENDIF()

SET( MYSOURCE
    main.cpp
    mainwindow/mainwindow.cpp
    mainwindow/mainwindow.h
    mainwindow/mainwindowform.ui
    osgwidget.h
    osgwidget.cpp
    osgwidgetTop.h
    osgwidgetTop.cpp
    osgwidgetside.h
    osgwidgetside.cpp
    osgwidgetfront.h
    osgwidgetfront.cpp
    outputwindow/outputwindow.h
    outputwindow/outputwindow.cpp
    outputwindow/outputwindow.ui
    block.cpp
    block.h
)

add_executable( ${PROJECT_NAME}
    ${MYSOURCE}
)

target_link_libraries( ${PROJECT_NAME}
    ${OPENSCENEGRAPH_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    Qt5::PrintSupport
)

set(TEST_NAME ${PROJECT_NAME}_UnitTests)

add_executable(${TEST_NAME}
    unittests.cpp
)

target_link_libraries(${TEST_NAME} gtest gtest_main)
