// SPDX-License-Identifier: GPL-2.0-or-later
#include "bridge/NullQgisBridge.h"

namespace modernqgis {

bool NullQgisBridge::isAvailable() const { return false; }
QString NullQgisBridge::qgisVersion() const { return {}; }
QMainWindow* NullQgisBridge::mainWindow() const { return nullptr; }
QList<QAction*> NullQgisBridge::discoverActions() const { return {}; }
QList<QDockWidget*> NullQgisBridge::discoverDockWidgets() const { return {}; }
int NullQgisBridge::seedCoreCommands(CommandRegistry&) const { return 0; }

} // namespace modernqgis
