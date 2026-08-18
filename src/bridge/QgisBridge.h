// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "bridge/IQgisBridge.h"

class QgisInterface;

namespace modernqgis {

class QgisBridge final : public IQgisBridge {
public:
    explicit QgisBridge(QgisInterface* iface);

    bool isAvailable() const override;
    QString qgisVersion() const override;
    QMainWindow* mainWindow() const override;
    QList<QAction*> discoverActions() const override;
    QList<QDockWidget*> discoverDockWidgets() const override;
    int seedCoreCommands(CommandRegistry& registry) const override;

    // Connects already-present ModernQGIS presentation actions to stable public
    // QgisInterface actions. This preserves the presentation/feature boundary.
    int bindRegisteredCommands(CommandRegistry& registry) const;

private:
    QgisInterface* m_iface = nullptr;
};

} // namespace modernqgis
