// SPDX-License-Identifier: GPL-2.0-or-later
#include "sandbox/SandboxWindow.h"
#include "core/CommandDescriptor.h"
#include "core/PaneDescriptor.h"
#include <QAction>
#include <QDockWidget>
#include <QLabel>
#include <QListWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

namespace modernqgis {
SandboxWindow::SandboxWindow(QWidget* parent)
    : QMainWindow(parent), m_commands(this), m_panes(this), m_contexts(this) {
    buildShell();
    registerFoundationObjects();
}
void SandboxWindow::buildShell() {
    setWindowTitle(tr("ModernQGIS — Foundation Sandbox"));
    resize(1280, 800);
    auto* commandBar = addToolBar(tr("Commands"));
    commandBar->setMovable(false);
    commandBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    const auto addCommand = [this, commandBar](const QString& id, const QString& text) {
        auto* action = commandBar->addAction(text);
        CommandDescriptor descriptor;
        descriptor.id = id;
        descriptor.title = text;
        descriptor.category = QStringLiteral("foundation");
        descriptor.action = action;
        m_commands.registerCommand(descriptor);
    };
    addCommand(QStringLiteral("project.new"), tr("New Project"));
    addCommand(QStringLiteral("project.open"), tr("Open Project"));
    commandBar->addSeparator();
    addCommand(QStringLiteral("map.add-data"), tr("Add Data"));
    addCommand(QStringLiteral("map.explore"), tr("Explore"));
    addCommand(QStringLiteral("map.full-extent"), tr("Full Extent"));

    auto* leftDock = new QDockWidget(tr("Contents"), this);
    leftDock->setObjectName(QStringLiteral("modernqgis.contents"));
    m_contents = new QListWidget(leftDock);
    m_contents->addItems({tr("Layers will appear here"), tr("QGIS bridge is not connected in sandbox mode")});
    leftDock->setWidget(m_contents);
    addDockWidget(Qt::LeftDockWidgetArea, leftDock);

    auto* rightDock = new QDockWidget(tr("Properties"), this);
    rightDock->setObjectName(QStringLiteral("modernqgis.properties"));
    m_properties = new QLabel(tr("Select an object to inspect its properties."), rightDock);
    m_properties->setWordWrap(true);
    m_properties->setMargin(16);
    rightDock->setWidget(m_properties);
    addDockWidget(Qt::RightDockWidgetArea, rightDock);

    m_workspace = new QTabWidget(this);
    m_workspace->setTabsClosable(true);
    m_workspace->setMovable(true);
    auto* mapPlaceholder = new QWidget(m_workspace);
    auto* mapLayout = new QVBoxLayout(mapPlaceholder);
    auto* mapLabel = new QLabel(tr("Map workspace placeholder"), mapPlaceholder);
    mapLabel->setAlignment(Qt::AlignCenter);
    mapLayout->addWidget(mapLabel);
    m_workspace->addTab(mapPlaceholder, tr("Map 1"));
    setCentralWidget(m_workspace);
    statusBar()->showMessage(tr("Foundation mode — QGIS integration disabled"));
}
void SandboxWindow::registerFoundationObjects() {
    PaneDescriptor contentsPane;
    contentsPane.id = QStringLiteral("contents");
    contentsPane.title = tr("Contents");
    contentsPane.preferredLocation = PaneLocation::Left;
    contentsPane.widget = m_contents;
    m_panes.registerPane(contentsPane);
    PaneDescriptor propertiesPane;
    propertiesPane.id = QStringLiteral("properties");
    propertiesPane.title = tr("Properties");
    propertiesPane.preferredLocation = PaneLocation::Right;
    propertiesPane.widget = m_properties;
    m_panes.registerPane(propertiesPane);
    m_contexts.activate(QStringLiteral("workspace.map"));
}
} // namespace modernqgis
