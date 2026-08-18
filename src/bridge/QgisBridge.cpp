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
namespace {

struct Binding {
    const char* id;
    const char* category;
    QAction* action;
};

QList<Binding> publicBindings(QgisInterface* iface) {
    if (!iface) return {};
    return {
        {"project.new", "project", iface->actionNewProject()},
        {"project.open", "project", iface->actionOpenProject()},
        {"project.save", "project", iface->actionSaveProject()},
        {"map.explore", "map", iface->actionPan()},
        {"map.pan", "map", iface->actionPan()},
        {"map.zoom-in", "map", iface->actionZoomIn()},
        {"map.zoom-out", "map", iface->actionZoomOut()},
        {"map.full-extent", "map", iface->actionZoomFullExtent()},
        {"map.identify", "map", iface->actionIdentify()},
        {"map.measure", "map", iface->actionMeasure()},
        {"map.measure-area", "map", iface->actionMeasureArea()},
        {"map.zoom-selection", "map", iface->actionZoomToSelected()},
        {"layer.add-vector", "layer", iface->actionAddOgrLayer()},
        {"layer.add-raster", "layer", iface->actionAddRasterLayer()},
        {"layer.attribute-table", "layer", iface->actionOpenTable()},
        {"layer.properties", "layer", iface->actionLayerProperties()},
        {"layer.toggle-editing", "edit", iface->actionToggleEditing()},
    };
}

} // namespace

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
    int added = 0;
    for (const auto& binding : publicBindings(m_iface)) {
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

int QgisBridge::bindRegisteredCommands(CommandRegistry& registry) const {
    int bound = 0;
    for (const auto& binding : publicBindings(m_iface)) {
        if (!binding.action) continue;
        const auto id = QString::fromLatin1(binding.id);
        if (!registry.contains(id)) continue;
        const auto descriptor = registry.command(id);
        if (!descriptor.action) continue;
        QAction* shellAction = descriptor.action.data();
        QAction* nativeAction = binding.action;
        QObject::connect(shellAction, &QAction::triggered, nativeAction, [nativeAction] {
            nativeAction->trigger();
        }, Qt::UniqueConnection);
        shellAction->setEnabled(nativeAction->isEnabled());
        shellAction->setCheckable(nativeAction->isCheckable());
        shellAction->setChecked(nativeAction->isChecked());
        QObject::connect(nativeAction, &QAction::changed, shellAction, [shellAction, nativeAction] {
            shellAction->setEnabled(nativeAction->isEnabled());
            if (shellAction->isCheckable()) shellAction->setChecked(nativeAction->isChecked());
        }, Qt::UniqueConnection);
        ++bound;
    }
    return bound;
}

} // namespace modernqgis
