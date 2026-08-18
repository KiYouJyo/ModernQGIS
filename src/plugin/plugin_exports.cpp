// SPDX-License-Identifier: GPL-2.0-or-later
#include "plugin/ModernQgisPlugin.h"
#include <qgis.h>
#include <qgisinterface.h>
#include <qgisplugin.h>

namespace {
const QString kName = QStringLiteral("ModernQGIS");
const QString kDescription = QStringLiteral("Modern desktop shell architecture for QGIS");
const QString kCategory = QStringLiteral("Interface");
const QString kVersion = QStringLiteral(MODERNQGIS_VERSION);
const QString kIcon;
const QString kExperimental = QStringLiteral("true");
}
QGISEXTERN QgisPlugin* classFactory(QgisInterface* iface) { return new modernqgis::ModernQgisPlugin(iface); }
QGISEXTERN const QString* name() { return &kName; }
QGISEXTERN const QString* description() { return &kDescription; }
QGISEXTERN const QString* category() { return &kCategory; }
QGISEXTERN const QString* version() { return &kVersion; }
QGISEXTERN int type() { return QgisPlugin::UI; }
QGISEXTERN const QString* icon() { return &kIcon; }
QGISEXTERN const QString* experimental() { return &kExperimental; }
QGISEXTERN void unload(QgisPlugin* plugin) { delete plugin; }
