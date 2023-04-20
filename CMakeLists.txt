cmake_minimum_required(VERSION 3.5)

project(Cytometrie_en_flux LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT5_LIBRARIES Qt5::Widgets Qt5::PrintSupport Qt5::Charts)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Qt5Charts)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.hpp
    src/qcustomplot.cpp
    include/qcustomplot.h
    src/DataStruct.cpp
    include/DataStruct.hpp
    src/VisualData.cpp
    include/VisualData.hpp
    src/DataReader.cpp
    include/DataReader.hpp
    src/File.cpp
    include/File.hpp
    src/QPlot.cpp
    include/QPlot.hpp
    src/QGraph.cpp
    include/QGraph.hpp
    src/GraphWindow.cpp
    include/GraphWindow.hpp
    src/GraphWindow.ui
)

add_executable(Cytometrie_en_flux
    ${PROJECT_SOURCES}
)

target_include_directories(Cytometrie_en_flux PRIVATE ./include)
target_include_directories(Cytometrie_en_flux PRIVATE ./ui)

target_link_libraries(Cytometrie_en_flux ${QT5_LIBRARIES})

add_compile_definitions(QCUSTOMPLOT_USE_OPENGL)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(Cytometrie_en_flux ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})