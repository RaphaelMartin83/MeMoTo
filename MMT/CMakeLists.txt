cmake_minimum_required(VERSION 3.5)

set(MEMOTO_VERSION_MAJOR "0" CACHE STRING "MeMoTo version major")
set(MEMOTO_VERSION_MINOR "0" CACHE STRING "MeMoTo version minor")
set(MEMOTO_VERSION_PATCH "0" CACHE STRING "MeMoTo version patch")

option(SET_VERSION_MAJOR "${MEMOTO_VERSION_MAJOR}" "Set MeMoTo version major")
option(SET_VERSION_MINOR "${MEMOTO_VERSION_MINOR}" "Set MeMoTo version minor")
option(SET_VERSION_PATCH "${MEMOTO_VERSION_PATCH}" "Set MeMoTo version patch")

set(MEMOTO_VERSION_FULL "${MEMOTO_VERSION_MAJOR}.${MEMOTO_VERSION_MINOR}.${MEMOTO_VERSION_PATCH}" CACHE STRING "MeMoTo full version")

project(MMT VERSION ${MEMOTO_VERSION_MAJOR}.${MEMOTO_VERSION_MINOR} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Network WebSockets Svg)

set(TS_FILES MMT_en_US.ts)

file(GLOB COMMON_GRAPHICS_HEADERS "CommonGraphics/*.h")
file(GLOB COMMON_GRAPHICS_SOURCES "CommonGraphics/*.cpp")
file(GLOB DIAGRAM_SCENES_HEADERS "DiagramScenes/*.h")
file(GLOB DIAGRAM_SCENES_SOURCES "DiagramScenes/*.cpp")
file(GLOB GRAPHICS_ITEMS_HEADERS "GraphicsItems/*.h")
file(GLOB GRAPHICS_ITEMS_SOURCES "GraphicsItems/*.cpp")
file(GLOB TOOLBOX_SCENES_HEADERS "ToolBoxScenes/*.h")
file(GLOB TOOLBOX_SCENES_SOURCES "ToolBoxScenes/*.cpp")
file(GLOB CONFIGURATION_CONTEXTS_HEADERS "ConfigurationContexts/*.h")
file(GLOB CONFIGURATION_CONTEXTS_SOURCES "ConfigurationContexts/*.cpp")
file(GLOB ENGINE_HEADERS "Engine/*.h")
file(GLOB ENGINE_SOURCES "Engine/*.cpp")
file(GLOB SHARING_HEADERS "Sharing/*.h")
file(GLOB SHARING_SOURCES "Sharing/*.cpp")

set(PROJECT_SOURCES
    main.cpp
    MeMoToApplication.h MeMoToApplication.cpp MeMoToThemeDefinition.h
    MainWindow.h MainWindow.cpp
    ${COMMON_GRAPHICS_HEADERS} ${COMMON_GRAPHICS_SOURCES}
    ${DIAGRAM_SCENES_HEADERS} ${DIAGRAM_SCENES_SOURCES}
    ${GRAPHICS_ITEMS_HEADERS} ${GRAPHICS_ITEMS_SOURCES}
    ${TOOLBOX_SCENES_HEADERS} ${TOOLBOX_SCENES_SOURCES}
    ${CONFIGURATION_CONTEXTS_HEADERS} ${CONFIGURATION_CONTEXTS_SOURCES}
    ${ENGINE_HEADERS} ${ENGINE_SOURCES}
    ${SHARING_HEADERS} ${SHARING_SOURCES}
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MMT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MMT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(MMT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MMT
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_compile_definitions(MMT PRIVATE MEMOTO_VERSION_FULL="${MEMOTO_VERSION_FULL}")

target_link_libraries(MMT
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Network
	Qt${QT_VERSION_MAJOR}::WebSockets
	Qt${QT_VERSION_MAJOR}::Svg
)

set_target_properties(MMT PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MMT)
endif()

add_subdirectory(tests)
