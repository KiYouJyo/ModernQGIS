// SPDX-License-Identifier: GPL-2.0-or-later
#include "core/PaneRegistry.h"

namespace modernqgis {

PaneRegistry::PaneRegistry(QObject* parent)
    : QObject(parent) {}

bool PaneRegistry::registerPane(const PaneDescriptor& pane) {
    const auto id = pane.id.trimmed();
    if (id.isEmpty() || m_panes.contains(id)) {
        return false;
    }

    auto normalized = pane;
    normalized.id = id;
    m_panes.insert(id, normalized);
    emit paneRegistered(id);
    return true;
}

bool PaneRegistry::unregisterPane(const QString& id) {
    if (m_panes.remove(id) == 0) {
        return false;
    }

    emit paneUnregistered(id);
    return true;
}

bool PaneRegistry::contains(const QString& id) const {
    return m_panes.contains(id);
}

PaneDescriptor PaneRegistry::pane(const QString& id) const {
    return m_panes.value(id);
}

QVector<PaneDescriptor> PaneRegistry::panes() const {
    QVector<PaneDescriptor> result;
    result.reserve(m_panes.size());
    for (const auto& pane : m_panes) {
        result.push_back(pane);
    }
    return result;
}

int PaneRegistry::size() const {
    return m_panes.size();
}

} // namespace modernqgis
