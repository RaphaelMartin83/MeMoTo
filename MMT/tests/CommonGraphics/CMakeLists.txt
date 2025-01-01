cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

file(GLOB headers "*.h")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets LinguistTools Svg Qt6Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets LinguistTools Network WebSockets Svg Test)

foreach(header ${headers})
    get_filename_component(executable_name "${header}" NAME_WE)
    add_executable(${executable_name} ${header} ${executable_name}.cpp)
    add_test(${executable_name} ${executable_name})

    target_link_libraries(${executable_name}
      PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
	Qt${QT_VERSION_MAJOR}::Test
    )

    if(QT_VERSION_MAJOR EQUAL 6)
      qt_finalize_executable(${executable_name})
    endif()

endforeach()
