// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include "core/CommandRegistry.h"
#include "core/ContextRegistry.h"
#include "core/PaneRegistry.h"
#include <QMainWindow>
class QLabel; class QListWidget; class QTabWidget;
namespace modernqgis {
class SandboxWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit SandboxWindow(QWidget* parent = nullptr);
private:
    void buildShell();
    void registerFoundationObjects();
    CommandRegistry m_commands;
    PaneRegistry m_panes;
    ContextRegistry m_contexts;
    QListWidget* m_contents = nullptr;
    QTabWidget* m_workspace = nullptr;
    QLabel* m_properties = nullptr;
};
} // namespace modernqgis
