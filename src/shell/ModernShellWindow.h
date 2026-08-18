// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "core/CommandRegistry.h"
#include "core/ContextRegistry.h"
#include "core/PaneRegistry.h"

#include <QMainWindow>

class QAction;
class QCloseEvent;
class QLineEdit;
class QSplitter;
class QTabWidget;
class QTreeWidget;
class QWidget;

namespace modernqgis {

class ModernShellWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit ModernShellWindow(QWidget* parent = nullptr);

    CommandRegistry& commandRegistry() { return m_commands; }
    PaneRegistry& paneRegistry() { return m_panes; }
    ContextRegistry& contextRegistry() { return m_contexts; }

    // Generic integration points used by runtime bridges. These deliberately
    // accept Qt widgets/actions only so the shell remains independent from
    // QGIS API types and version churn.
    QAction* commandAction(const QString& id) const;
    void replaceMapCanvas(QWidget* widget);
    void replaceContentsTree(QWidget* widget);
    void setProjectDisplayName(const QString& name);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    QWidget* buildTitleBar();
    QWidget* buildRibbonTabs();
    QWidget* buildRibbonWorkspace();
    QWidget* buildContentsPane();
    QWidget* buildMapWorkspace();
    QWidget* buildPropertiesPane();
    QWidget* buildStatusBar();
    QWidget* makeRibbonGroup(const QString& title, const QList<QPair<QString, QString>>& commands);
    void registerShellCommand(const QString& id, const QString& title, QAction* action, const QString& category);
    void setRibbonCollapsed(bool collapsed);
    void restoreShellState();
    void saveShellState() const;

    CommandRegistry m_commands;
    PaneRegistry m_panes;
    ContextRegistry m_contexts;
    QLineEdit* m_globalSearch = nullptr;
    QWidget* m_ribbonWorkspace = nullptr;
    QSplitter* m_workspaceSplitter = nullptr;
    QTreeWidget* m_layerTree = nullptr;
    QTabWidget* m_documentTabs = nullptr;
    bool m_ribbonCollapsed = false;
};

} // namespace modernqgis
