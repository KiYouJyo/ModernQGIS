// SPDX-License-Identifier: GPL-2.0-or-later
#include "plugin/ModernQgisPlugin.h"

#include "bridge/QgisBridge.h"
#include "shell/ModernShellWindow.h"

#include <qgisinterface.h>
#include <QAction>

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
    m_action = new QAction(tr("Open ModernQGIS Shell Preview"), this);
    connect(m_action, &QAction::triggered, this, &ModernQgisPlugin::showShell);
    m_iface->addPluginToMenu(tr("&ModernQGIS"), m_action);
}

void ModernQgisPlugin::showShell() {
    if (!m_iface) return;
    if (!m_shell) {
        m_shell = new ModernShellWindow(m_iface->mainWindow());
        m_shell->setAttribute(Qt::WA_DeleteOnClose, false);

        m_bridge = new QgisBridge(m_iface, this);
        m_bridge->attachWorkspace(m_shell);
        m_bridge->bindRegisteredCommands(m_shell->commandRegistry());
    }
    m_shell->show();
    m_shell->raise();
    m_shell->activateWindow();
}

void ModernQgisPlugin::unload() {
    // Destroy bridge-owned map tools/models while the shell widgets they
    // reference are still alive.
    if (m_bridge) {
        delete m_bridge;
        m_bridge = nullptr;
    }
    if (m_shell) {
        m_shell->close();
        delete m_shell;
        m_shell = nullptr;
    }
    if (!m_iface || !m_action) return;
    m_iface->removePluginMenu(tr("&ModernQGIS"), m_action);
    delete m_action;
    m_action = nullptr;
}

} // namespace modernqgis
