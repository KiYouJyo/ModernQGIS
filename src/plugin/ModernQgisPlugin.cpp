// SPDX-License-Identifier: GPL-2.0-or-later
#include "plugin/ModernQgisPlugin.h"

#include "bridge/QgisBridge.h"
#include "shell/ModernShellWindow.h"
#include "shell/V04CommandSurface.h"

#include <qgisinterface.h>
#include <QAction>
#include <QTimer>
#include <QtGlobal>

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
    m_action = new QAction(tr("Open ModernQGIS"), this);
    connect(m_action, &QAction::triggered, this, &ModernQgisPlugin::showShell);
    m_iface->addPluginToMenu(tr("&ModernQGIS"), m_action);

    // The signed launcher sets this flag so an enabled ModernQGIS plugin opens
    // its shell as soon as QGIS has finished creating the plugin GUI.
    if (qEnvironmentVariableIntValue("MODERNQGIS_AUTO_OPEN") == 1) {
        QTimer::singleShot(0, this, &ModernQgisPlugin::showShell);
    }
}

void ModernQgisPlugin::showShell() {
    if (!m_iface) return;
    if (!m_shell) {
        m_shell = new ModernShellWindow(m_iface->mainWindow());
        m_shell->setAttribute(Qt::WA_DeleteOnClose, false);
        installV04CommandSurface(m_shell);

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
