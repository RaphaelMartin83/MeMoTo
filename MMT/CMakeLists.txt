cmake_minimum_required(VERSION 3.5)

project(MMT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES MMT_en_US.ts)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/MeMoTo.res")

set(PROJECT_SOURCES
        main.cpp
        MeMoToApplication.h MeMoToApplication.cpp
        MainWindow.h MainWindow.cpp I_DataManager.h
        SharingManager.h SharingManager.cpp
        SharingConfiguration.h SharingConfiguration.cpp I_SharingConfigurationListener.h
        InstanceLauncher.h InstanceLauncher.cpp
        DiagramGraphicsView.h DiagramGraphicsView.cpp
        GridReferential.h GridReferential.cpp
        SaveFileConfiguration.h SaveFileConfiguration.cpp I_SaveFileConfigurationListener.h
        LoadFileConfiguration.h LoadFileConfiguration.cpp I_LoadFileConfigurationListener.h
        FindConfiguration.h FindConfiguration.cpp I_FindConfigurationListener.h
        I_Selectable.h
        I_Connectable.h
        I_ConnectableContainer.h I_ConnectableContainer.cpp
        I_ToolSwitcherListener.h I_ToolListener.h
        I_Configurable.h
        I_ConfigurationContent.h I_ConfigurationContent.cpp
        I_GraphicsItem.h
        I_Tool.h
        I_Undoable.h
        MouseTool.h MouseTool.cpp
        ConnectionDefinitions.h
        StackingDefinitions.h
        I_Serializable.h
        I_DiagramContainer.h I_DiagramContainer.cpp I_ToolBox.h
        ConnectableTool.h ConnectorTool.h ContainerTool.h
        SMDiagramScene.h SMDiagramScene.cpp
        SMToolBoxScene.h SMToolBoxScene.cpp
        ClassDiagramScene.h ClassDiagramScene.cpp
        ClassToolBoxScene.h ClassToolBoxScene.cpp
        MouseToolPolygon.h MouseToolPolygon.cpp
        I_SquarishGraphicsItem.h I_SquarishGraphicsItem.cpp
        I_ClassGraphicsItem.h I_ClassGraphicsItem.cpp
        I_ContainerGraphicsItem.h I_ContainerGraphicsItem.cpp I_ContainersContainer.h
        ClassGraphicsItem.h ClassGraphicsItem.cpp
        ClassConfiguration.h ClassConfiguration.cpp
        ExternalClassGraphicsItem.h ExternalClassGraphicsItem.cpp
        ExternalClassConfiguration.h ExternalClassConfiguration.cpp
        StateGraphicsItem.h StateGraphicsItem.cpp
        StateConfiguration.h StateConfiguration.cpp
        ChoiceGraphicsItem.h ChoiceGraphicsItem.cpp
        ChoiceConfiguration.h ChoiceConfiguration.cpp
        InitialStateGraphicsItem.h InitialStateGraphicsItem.cpp
        EntryStateGraphicsItem.h EntryStateGraphicsItem.cpp
        HandleGraphicsItem.h HandleGraphicsItem.cpp
        Connector.h Connector.cpp I_Connector.h
        GraphicConnector.h GraphicConnector.cpp
        ConfigWidget.h ConfigWidget.cpp
        TransitionGraphicsItem.h TransitionGraphicsItem.cpp
        TransitionConfiguration.h TransitionConfiguration.cpp
        TransitionGraphicsLabel.h TransitionGraphicsLabel.cpp
        GeneralizationGraphicsItem.h GeneralizationGraphicsItem.cpp
        GeneralizationConfiguration.h GeneralizationConfiguration.cpp
        CompositionGraphicsItem.h CompositionGraphicsItem.cpp
        CompositionConfiguration.h CompositionConfiguration.cpp
        AggregationGraphicsItem.h AggregationGraphicsItem.cpp
        AggregationConfiguration.h AggregationConfiguration.cpp
        ImplementationGraphicsItem.h ImplementationGraphicsItem.cpp
        ImplementationConfiguration.h ImplementationConfiguration.cpp
        DoubleArrowGraphicsItem.h DoubleArrowGraphicsItem.cpp
        HumanReadableColor.h HumanReadableColor.cpp
        ConfigLineEdit.h ConfigLineEdit.cpp
        ConfigList.h ConfigList.cpp
        I_ConfigLineEditListener.h
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

target_link_libraries(MMT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

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
