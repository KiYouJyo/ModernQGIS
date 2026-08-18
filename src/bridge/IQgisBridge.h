// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QList>
#include <QString>

class QAction;
class QDockWidget;
class QMainWindow;

namespace modernqgis {

class CommandRegistry;

class IQgisBridge {
public:
    virtual ~IQgisBridge() = default;

    virtual bool isAvailable() const = 0;
    virtual QString qgisVersion() const = 0;
    virtual QMainWindow* mainWindow() const = 0;
    virtual QList<QAction*> discoverActions() const = 0;
    virtual QList<QDockWidget*> discoverDockWidgets() const = 0;
    virtual int seedCoreCommands(CommandRegistry& registry) const = 0;
};

} // namespace modernqgis
