// SPDX-License-Identifier: GPL-2.0-or-later
#include "shell/ModernShellWindow.h"

#include <QAction>
#include <QLabel>
#include <QLayout>
#include <QTabWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

namespace modernqgis {

QAction* ModernShellWindow::commandAction(const QString& id) const {
    if (!m_commands.contains(id)) return nullptr;
    return m_commands.command(id).action.data();
}

void ModernShellWindow::replaceMapCanvas(QWidget* widget) {
    if (!widget || !m_documentTabs) return;

    const int mapIndex = 0;
    QWidget* previous = m_documentTabs->widget(mapIndex);
    const QString title = m_documentTabs->tabText(mapIndex);
    if (previous == widget) return;

    m_documentTabs->removeTab(mapIndex);
    widget->setParent(m_documentTabs);
    m_documentTabs->insertTab(mapIndex, widget, title);
    m_documentTabs->setCurrentIndex(mapIndex);

    if (previous) {
        previous->hide();
        previous->deleteLater();
    }
}

void ModernShellWindow::replaceContentsTree(QWidget* widget) {
    if (!widget || !m_layerTree) return;

    QWidget* pane = m_layerTree->parentWidget();
    auto* layout = pane ? qobject_cast<QVBoxLayout*>(pane->layout()) : nullptr;
    if (!layout) return;

    const int index = layout->indexOf(m_layerTree);
    layout->removeWidget(m_layerTree);
    widget->setParent(pane);
    layout->insertWidget(index < 0 ? layout->count() : index, widget, 1);

    m_layerTree->hide();
    m_layerTree->deleteLater();
    m_layerTree = nullptr;
}

void ModernShellWindow::setProjectDisplayName(const QString& name) {
    const QString effectiveName = name.trimmed();
    const QString caption = effectiveName.isEmpty()
        ? QStringLiteral("ModernQGIS")
        : QStringLiteral("ModernQGIS · %1").arg(effectiveName);

    if (auto* title = findChild<QLabel*>(QStringLiteral("WindowTitle"))) {
        title->setText(caption);
    }
    setWindowTitle(caption);
}

} // namespace modernqgis
