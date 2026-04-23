cmake_minimum_required(VERSION 3.16)

project(ComputerVisionManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)
if(!${PKG_CONFIG_FOUND})
  MESSAGE(FATAL_ERROR "pkg-config required. Please install it")
  return ("Error - pkg-config not found")
endif()

pkg_check_modules(GSTREAMER gstreamer-1.0>=1.6 gstreamer-video-1.0>=1.6 REQUIRED)


find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Qml
    Quick
    Gamepad
    RemoteObjects
)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        remotepipeline.rep
        remotevehicle.rep
        remotenetwork.rep
)

qt5_generate_repc(PROJECT_SOURCES remotepipeline.rep REPLICA)
qt5_generate_repc(PROJECT_SOURCES remotevehicle.rep REPLICA)
qt5_generate_repc(PROJECT_SOURCES remotenetwork.rep REPLICA)

link_directories(${GSTREAMER_LIBRARY_DIRS})

include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(ComputerVisionManager
    ${PROJECT_SOURCES}
    videopipeline.h videopipeline.cpp
    videocontroller.h videocontroller.cpp
    videosyncstart.h videosyncstart.cpp
    gamepadhandler.h gamepadhandler.cpp
    controlworker.h controlworker.cpp
    network.h network.cpp

    remotepipeline.rep
    remotevehicle.rep
    remotenetwork.rep

    dirtransfer.h dirtransfer.cpp
)


target_link_libraries(ComputerVisionManager
    PRIVATE Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Gamepad
    Qt${QT_VERSION_MAJOR}::RemoteObjects
    ${GSTREAMER_LIBRARIES}
)

set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ComputerVisionManager)

set_target_properties(ComputerVisionManager PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ComputerVisionManager
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(ComputerVisionManager)
    qt_finalize_executable(ComputerVisionManager)
endif()

# Testing Git and Github
