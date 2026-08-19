cmake_minimum_required(VERSION 3.22)

project(
    ModernQGIS
    VERSION 0.4.0
    DESCRIPTION "Modern desktop shell and interaction architecture for QGIS"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(GNUInstallDirs)
include(CTest)

option(MODERNQGIS_BUILD_SANDBOX "Build the standalone UI sandbox" ON)
option(MODERNQGIS_BUILD_TESTS "Build foundation tests" ON)
option(MODERNQGIS_WITH_QGIS "Build the QGIS bridge/plugin integration" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt6 6.6 REQUIRED COMPONENTS Core Gui Widgets Xml LinguistTools Test)

if(MODERNQGIS_WITH_QGIS)
    find_package(QGIS 4.0 REQUIRED)
endif()

add_subdirectory(src)

set(MODERNQGIS_TS_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/modernqgis_zh_CN.ts"
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/modernqgis_ja_JP.ts"
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/modernqgis_en_US.ts"
)

if(TARGET modernqgis-sandbox)
    qt_add_translations(
        modernqgis-sandbox
        TS_FILES ${MODERNQGIS_TS_FILES}
        RESOURCE_PREFIX "/i18n"
    )
endif()

if(TARGET modernqgis_plugin)
    qt_add_translations(
        modernqgis_plugin
        TS_FILES ${MODERNQGIS_TS_FILES}
        RESOURCE_PREFIX "/i18n"
    )
endif()

if(TARGET modernqgis-launcher)
    qt_add_translations(
        modernqgis-launcher
        TS_FILES ${MODERNQGIS_TS_FILES}
        RESOURCE_PREFIX "/i18n"
    )
endif()

if(BUILD_TESTING AND MODERNQGIS_BUILD_TESTS)
    add_subdirectory(tests)
endif()

install(
    DIRECTORY docs/
    DESTINATION "${CMAKE_INSTALL_DATADIR}/modernqgis/docs"
)
