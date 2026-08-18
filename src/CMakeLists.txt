set(MODERNQGIS_FOUNDATION_SOURCES
    core/CommandRegistry.cpp
    core/ContextRegistry.cpp
    core/PaneRegistry.cpp
    bridge/NullQgisBridge.cpp
    i18n/LocalizationManager.cpp
    shell/ShellStyle.cpp
    shell/ModernShellWindow.cpp
    shell/ModernShellIntegration.cpp
)

set(MODERNQGIS_FOUNDATION_HEADERS
    core/CommandDescriptor.h
    core/CommandRegistry.h
    core/ContextRegistry.h
    core/PaneDescriptor.h
    core/PaneRegistry.h
    bridge/IQgisBridge.h
    bridge/NullQgisBridge.h
    i18n/LocalizationManager.h
    shell/ShellStyle.h
    shell/ModernShellWindow.h
)

add_library(modernqgis_foundation STATIC
    ${MODERNQGIS_FOUNDATION_SOURCES}
    ${MODERNQGIS_FOUNDATION_HEADERS}
)

target_include_directories(modernqgis_foundation
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(modernqgis_foundation
    PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets
)

target_compile_definitions(modernqgis_foundation
    PUBLIC MODERNQGIS_VERSION="${PROJECT_VERSION}"
)

if(MODERNQGIS_BUILD_SANDBOX)
    add_executable(modernqgis-sandbox
        sandbox/main.cpp
        sandbox/SandboxWindow.cpp
        sandbox/SandboxWindow.h
    )
    target_link_libraries(modernqgis-sandbox PRIVATE modernqgis_foundation)
    install(TARGETS modernqgis-sandbox RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

if(MODERNQGIS_WITH_QGIS)
    add_library(modernqgis_qgis_bridge STATIC
        bridge/QgisBridge.cpp
        bridge/QgisBridge.h
    )
    target_link_libraries(modernqgis_qgis_bridge
        PUBLIC modernqgis_foundation QGIS::Core QGIS::Gui Qt6::Xml
    )

    add_library(modernqgis_plugin MODULE
        plugin/ModernQgisPlugin.cpp
        plugin/ModernQgisPlugin.h
        plugin/plugin_exports.cpp
    )
    target_link_libraries(modernqgis_plugin
        PRIVATE modernqgis_qgis_bridge QGIS::Core QGIS::Gui Qt6::Xml
    )
    set_target_properties(modernqgis_plugin PROPERTIES
        PREFIX ""
        OUTPUT_NAME "modernqgis"
    )
endif()
