// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "bridge/IQgisBridge.h"

namespace modernqgis {

class NullQgisBridge final : public IQgisBridge {
public:
    bool isAvailable() const override;
    QString qgisVersion() const override;
    QMainWindow* mainWindow() const override;
    QList<QAction*> discoverActions() const override;
    QList<QDockWidget*> discoverDockWidgets() const override;
    int seedCoreCommands(CommandRegistry& registry) const override;
};

} // namespace modernqgis
