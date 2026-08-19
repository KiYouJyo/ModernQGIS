// SPDX-License-Identifier: GPL-2.0-or-later
#include "shell/V04CommandSurface.h"

#include "core/CommandDescriptor.h"
#include "shell/ModernShellWindow.h"

#include <QAction>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

namespace modernqgis {
namespace {

QFrame* makeGroup(ModernShellWindow* shell,
                  const QString& title,
                  const QList<QPair<QString, QString>>& commands) {
    auto* group = new QFrame(shell);
    group->setObjectName(QStringLiteral("RibbonGroup"));
    auto* layout = new QVBoxLayout(group);
    layout->setContentsMargins(8, 7, 8, 4);
    layout->setSpacing(3);

    auto* row = new QHBoxLayout();
    row->setSpacing(2);
    for (const auto& item : commands) {
        if (shell->commandRegistry().contains(item.first)) continue;
        auto* action = new QAction(item.second, group);
        auto* button = new QToolButton(group);
        button->setDefaultAction(action);
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        button->setMinimumSize(54, 62);
        button->setAccessibleName(item.second);
        button->setToolTip(item.second);
        row->addWidget(button);

        CommandDescriptor descriptor;
        descriptor.id = item.first;
        descriptor.title = item.second;
        descriptor.category = title.toLower();
        descriptor.action = action;
        shell->commandRegistry().registerCommand(descriptor);
    }
    layout->addLayout(row, 1);
    auto* caption = new QLabel(title, group);
    caption->setObjectName(QStringLiteral("RibbonGroupTitle"));
    caption->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(caption);
    return group;
}

} // namespace

void installV04CommandSurface(ModernShellWindow* shell) {
    if (!shell) return;
    auto* ribbon = shell->findChild<QWidget*>(QStringLiteral("RibbonWorkspace"));
    auto* layout = ribbon ? qobject_cast<QHBoxLayout*>(ribbon->layout()) : nullptr;
    if (!layout) return;

    auto* project = makeGroup(shell, ModernShellWindow::tr("Project"), {
        {QStringLiteral("project.new"), ModernShellWindow::tr("New")},
        {QStringLiteral("project.open"), ModernShellWindow::tr("Open")},
        {QStringLiteral("project.save"), ModernShellWindow::tr("Save")},
    });
    auto* zoom = makeGroup(shell, ModernShellWindow::tr("Zoom"), {
        {QStringLiteral("map.zoom-in"), ModernShellWindow::tr("Zoom In")},
        {QStringLiteral("map.zoom-out"), ModernShellWindow::tr("Zoom Out")},
    });
    auto* edit = makeGroup(shell, ModernShellWindow::tr("Edit"), {
        {QStringLiteral("layer.toggle-editing"), ModernShellWindow::tr("Toggle Editing")},
    });

    layout->insertWidget(0, project);
    layout->insertWidget(1, zoom);
    const int stretchIndex = qMax(0, layout->count() - 1);
    layout->insertWidget(stretchIndex, edit);
}

} // namespace modernqgis
