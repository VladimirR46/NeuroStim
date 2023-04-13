cmake_minimum_required(VERSION 3.5)

project(NeuroStim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets QuickWidgets QuickControls2)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets QuickWidgets QuickControls2)


set(PROJECT_SOURCES
        main.cpp
        appengine.h
        appengine.cpp
        properties.cpp
        properties.h
        basetask.h
        basetask.cpp
        taskmanager.h
        taskmanager.cpp
        sample.h
        sample.cpp
        item.h
        item.cpp
        lsl.h
        lsl.cpp
        player.cpp
        player.h
        resources.qrc
        items/polygonitem.h
        items/polygonitem.cpp
        items/textitem.h
        items/textitem.cpp
        items/spiralitem.h
        items/spiralitem.cpp
        items/buttonitem.h
        items/buttonitem.cpp
        tasks/test1task.h
        tasks/test1task.cpp
        tasks/backgroundactivitytask.h
        tasks/backgroundactivitytask.cpp
        tasks/pointfindertask.h
        tasks/pointfindertask.cpp
        resource.rc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(NeuroStim
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
set_property(TARGET NeuroStim APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(NeuroStim SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(NeuroStim
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(NeuroStim PRIVATE Qt6::CorePrivate Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::QuickWidgets Qt${QT_VERSION_MAJOR}::QuickControls2)
include_directories("items" "tasks")

set_target_properties(NeuroStim PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
    WIN32_EXECUTABLE TRUE
)

install(TARGETS NeuroStim
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(NeuroStim)
endif()
