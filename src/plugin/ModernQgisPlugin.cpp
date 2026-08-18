// SPDX-License-Identifier: GPL-2.0-or-later
#include "plugin/ModernQgisPlugin.h"

#include <qgisinterface.h>
#include <QAction>
#include <QMessageBox>

namespace {
const QString kName = QStringLiteral("ModernQGIS");
const QString kDescription = QStringLiteral("Modern desktop shell architecture for QGIS");
const QString kCategory = QStringLiteral("Interface");
const QString kVersion = QStringLiteral(MODERNQGIS_VERSION);
}

namespace modernqgis {
ModernQgisPlugin::ModernQgisPlugin(QgisInterface* iface)
    : QgisPlugin(kName, kDescription, kCategory, kVersion, QgisPlugin::UI), m_iface(iface) {}
ModernQgisPlugin::~ModernQgisPlugin() { unload(); }
void ModernQgisPlugin::initGui() {
    if (!m_iface || m_action) return;
    m_action = new QAction(tr("ModernQGIS Shell Preview"), this);
    connect(m_action, &QAction::triggered, this, [this] {
        QMessageBox::information(m_iface->mainWindow(), tr("ModernQGIS"),
            tr("The v0.2 bridge is connected. Full shell replacement begins in v0.3."));
    });
    m_iface->addPluginToMenu(tr("&ModernQGIS"), m_action);
}
void ModernQgisPlugin::unload() {
    if (!m_iface || !m_action) return;
    m_iface->removePluginMenu(tr("&ModernQGIS"), m_action);
    delete m_action;
    m_action = nullptr;
}
} // namespace modernqgis
