cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

file(GLOB headers "*.h")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets LinguistTools Svg Qt6Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets LinguistTools Network WebSockets Svg Test)

# Automatically search the product source under test and fake sources besides that
foreach(header ${headers})
    get_filename_component(executable_name "${header}" NAME_WE)

    # Deduces source under test from test file name automatically
    # The test file name shall follow the following template:
    # -> If, for instance one wants to test file.cpp, then the test header shall be named Testfile.cpp
    string(REGEX REPLACE "^Test" "" source_under_test ${executable_name}.cpp)

    if(EXISTS ${CMAKE_SOURCE_DIR}/Engine/${source_under_test})
        set(absolute_source_under_test ${CMAKE_SOURCE_DIR}/Engine/${source_under_test})
    else()
        set(absolute_source_under_test "")
    endif()

    add_executable(
        ${executable_name}
        ${header} ${executable_name}.cpp ${absolute_source_under_test}
    )
    add_test(${executable_name} ${executable_name})

    target_include_directories(${executable_name} PRIVATE ${CMAKE_SOURCE_DIR})

    target_link_libraries(${executable_name}
      PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::WebSockets
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::Test
    )

    if(QT_VERSION_MAJOR EQUAL 6)
      qt_finalize_executable(${executable_name})
    endif()

endforeach()
