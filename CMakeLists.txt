cmake_minimum_required(VERSION 3.6)

project(Cytometrie_en_flux LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT6_LIBRARIES Qt6::Widgets Qt6::PrintSupport)

find_package(Qt6Widgets REQUIRED)
find_package(Qt6PrintSupport REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/qcustomplot.cpp
        include/qcustomplot.h
        src/csv.cpp
        include/csv.h
        include/mainwindow.h
        src/mainwindow.ui
)

add_executable(Cytometrie_en_flux
    ${PROJECT_SOURCES}
)

target_include_directories(Cytometrie_en_flux PRIVATE ./include)
target_include_directories(Cytometrie_en_flux PRIVATE ./ui)

target_link_libraries(Cytometrie_en_flux ${QT6_LIBRARIES})
