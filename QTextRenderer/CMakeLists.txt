cmake_minimum_required(VERSION 3.5)

project(QTextRenderer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem log)
find_package(fmt CONFIG REQUIRED)

set(TS_FILES QTextRenderer_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        textrenderer.cpp
        textrenderer.h
        textrenderer.ui
        textrenderer/bgmanager.h
        textrenderer/bgmanager.cpp
        textrenderer/fontmanager.h
        textrenderer/fontmanager.cpp
        textrenderer/stb_truetype.h
        textrenderer/stb_image_write.h
        textrenderer/utils/utils.h
        textrenderer/utils/utils.cpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QTextRenderer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QTextRenderer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(QTextRenderer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QTextRenderer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(QTextRenderer
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    ${OpenCV_LIBS}
    Boost::filesystem
    Boost::log
    fmt::fmt-header-only)

set_target_properties(QTextRenderer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTextRenderer)
endif()
