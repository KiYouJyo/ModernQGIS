// SPDX-License-Identifier: GPL-2.0-or-later
#include "bridge/QgisBridge.h"

#include "core/CommandDescriptor.h"
#include "core/CommandRegistry.h"

#include <qgis.h>
#include <qgisinterface.h>

#include <QAction>
#include <QDockWidget>
#include <QMainWindow>

namespace modernqgis {

QgisBridge::QgisBridge(QgisInterface* iface) : m_iface(iface) {}

bool QgisBridge::isAvailable() const { return m_iface != nullptr; }

QString QgisBridge::qgisVersion() const {
    return m_iface ? Qgis::version() : QString{};
}

QMainWindow* QgisBridge::mainWindow() const {
    return m_iface ? qobject_cast<QMainWindow*>(m_iface->mainWindow()) : nullptr;
}

QList<QAction*> QgisBridge::discoverActions() const {
    const auto* window = mainWindow();
    return window ? window->findChildren<QAction*>() : QList<QAction*>{};
}

QList<QDockWidget*> QgisBridge::discoverDockWidgets() const {
    const auto* window = mainWindow();
    return window ? window->findChildren<QDockWidget*>() : QList<QDockWidget*>{};
}

int QgisBridge::seedCoreCommands(CommandRegistry& registry) const {
    if (!m_iface) return 0;

    struct Binding { const char* id; const char* category; QAction* action; };
    const Binding bindings[] = {
        {"project.new", "project", m_iface->actionNewProject()},
        {"project.open", "project", m_iface->actionOpenProject()},
        {"project.save", "project", m_iface->actionSaveProject()},
        {"map.pan", "map", m_iface->actionPan()},
        {"map.zoom-in", "map", m_iface->actionZoomIn()},
        {"map.zoom-out", "map", m_iface->actionZoomOut()},
        {"map.full-extent", "map", m_iface->actionZoomFullExtent()},
        {"map.identify", "map", m_iface->actionIdentify()},
        {"map.measure", "map", m_iface->actionMeasure()},
        {"layer.add-vector", "layer", m_iface->actionAddOgrLayer()},
        {"layer.add-raster", "layer", m_iface->actionAddRasterLayer()},
        {"layer.toggle-editing", "edit", m_iface->actionToggleEditing()},
    };

    int added = 0;
    for (const auto& binding : bindings) {
        if (!binding.action) continue;
        CommandDescriptor descriptor;
        descriptor.id = QString::fromLatin1(binding.id);
        descriptor.title = binding.action->text();
        descriptor.description = binding.action->toolTip();
        descriptor.category = QString::fromLatin1(binding.category);
        descriptor.action = binding.action;
        if (registry.registerCommand(descriptor)) ++added;
    }
    return added;
}

} // namespace modernqgis
