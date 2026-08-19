# SPDX-License-Identifier: GPL-2.0-or-later
# Minimal public-header/library discovery for the optional ModernQGIS bridge.
# Supports distro installs and the OSGeo4W layouts used by QGIS 4.x on Windows.

set(_QGIS_HINTS)
if(QGIS_PREFIX_PATH)
    list(APPEND _QGIS_HINTS "${QGIS_PREFIX_PATH}")
    # OSGeo4W normally exposes QGIS as <root>/apps/qgis while import libraries
    # and other SDK pieces may also live below <root>. Keep both locations in
    # the search set without requiring the CI caller to know package internals.
    get_filename_component(_QGIS_APPS_DIR "${QGIS_PREFIX_PATH}" DIRECTORY)
    get_filename_component(_QGIS_OSGEO_ROOT "${_QGIS_APPS_DIR}" DIRECTORY)
    list(APPEND _QGIS_HINTS "${_QGIS_OSGEO_ROOT}")
endif()
if(DEFINED ENV{QGIS_PREFIX_PATH})
    list(APPEND _QGIS_HINTS "$ENV{QGIS_PREFIX_PATH}")
endif()
if(DEFINED ENV{OSGEO4W_ROOT})
    list(APPEND _QGIS_HINTS
        "$ENV{OSGEO4W_ROOT}"
        "$ENV{OSGEO4W_ROOT}/apps/qgis"
        "$ENV{OSGEO4W_ROOT}/apps/qgis-ltr"
        "$ENV{OSGEO4W_ROOT}/apps/qgis-dev"
    )
endif()
list(REMOVE_DUPLICATES _QGIS_HINTS)

find_path(
    QGIS_INCLUDE_DIR
    NAMES qgis.h qgsapplication.h
    HINTS ${_QGIS_HINTS}
    PATH_SUFFIXES include include/qgis apps/qgis/include apps/qgis-ltr/include apps/qgis-dev/include
)
find_library(
    QGIS_CORE_LIBRARY
    NAMES qgis_core
    HINTS ${_QGIS_HINTS}
    PATH_SUFFIXES lib bin apps/qgis/lib apps/qgis/bin apps/qgis-ltr/lib apps/qgis-ltr/bin apps/qgis-dev/lib apps/qgis-dev/bin
)
find_library(
    QGIS_GUI_LIBRARY
    NAMES qgis_gui
    HINTS ${_QGIS_HINTS}
    PATH_SUFFIXES lib bin apps/qgis/lib apps/qgis/bin apps/qgis-ltr/lib apps/qgis-ltr/bin apps/qgis-dev/lib apps/qgis-dev/bin
)

# QGIS packaging differs slightly across platforms. In particular, OSGeo4W
# development packages can expose the version macros in qgsversion.h instead
# of the headers historically inspected by this finder.
set(QGIS_VERSION "")
if(QGIS_INCLUDE_DIR)
    foreach(_candidate qgsversion.h qgsconfig.h qgis.h)
        if(EXISTS "${QGIS_INCLUDE_DIR}/${_candidate}")
            file(
                STRINGS "${QGIS_INCLUDE_DIR}/${_candidate}"
                _qgis_version_lines
                REGEX "(QGIS_VERSION|VERSION).*[0-9]+\\.[0-9]+"
            )
            foreach(_line IN LISTS _qgis_version_lines)
                string(REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?" _qgis_version_match "${_line}")
                if(_qgis_version_match)
                    set(QGIS_VERSION "${_qgis_version_match}")
                    break()
                endif()
            endforeach()
            if(QGIS_VERSION)
                break()
            endif()
        endif()
    endforeach()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    QGIS
    REQUIRED_VARS QGIS_INCLUDE_DIR QGIS_CORE_LIBRARY QGIS_GUI_LIBRARY
    VERSION_VAR QGIS_VERSION
)

if(QGIS_FOUND AND NOT TARGET QGIS::Core)
    add_library(QGIS::Core UNKNOWN IMPORTED)
    set_target_properties(
        QGIS::Core PROPERTIES
        IMPORTED_LOCATION "${QGIS_CORE_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${QGIS_INCLUDE_DIR}"
    )
endif()
if(QGIS_FOUND AND NOT TARGET QGIS::Gui)
    add_library(QGIS::Gui UNKNOWN IMPORTED)
    set_target_properties(
        QGIS::Gui PROPERTIES
        IMPORTED_LOCATION "${QGIS_GUI_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${QGIS_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES QGIS::Core
    )
endif()

mark_as_advanced(QGIS_INCLUDE_DIR QGIS_CORE_LIBRARY QGIS_GUI_LIBRARY)
