// SPDX-License-Identifier: GPL-2.0-or-later
#include "core/ContextRegistry.h"

namespace modernqgis {

ContextRegistry::ContextRegistry(QObject* parent)
    : QObject(parent) {}

bool ContextRegistry::activate(const QString& context) {
    const auto normalized = context.trimmed();
    if (normalized.isEmpty() || m_activeContexts.contains(normalized)) {
        return false;
    }

    m_activeContexts.insert(normalized);
    emit contextsChanged();
    return true;
}

bool ContextRegistry::deactivate(const QString& context) {
    if (!m_activeContexts.remove(context)) {
        return false;
    }

    emit contextsChanged();
    return true;
}

bool ContextRegistry::isActive(const QString& context) const {
    return m_activeContexts.contains(context);
}

QStringList ContextRegistry::activeContexts() const {
    auto result = m_activeContexts.values();
    result.sort();
    return result;
}

} // namespace modernqgis
