// SPDX-License-Identifier: GPL-2.0-or-later
#include "core/CommandRegistry.h"

namespace modernqgis {

CommandRegistry::CommandRegistry(QObject* parent)
    : QObject(parent) {}

bool CommandRegistry::registerCommand(const CommandDescriptor& command) {
    const auto id = command.id.trimmed();
    if (id.isEmpty() || m_commands.contains(id)) {
        return false;
    }

    auto normalized = command;
    normalized.id = id;
    m_commands.insert(id, normalized);
    emit commandRegistered(id);
    return true;
}

bool CommandRegistry::unregisterCommand(const QString& id) {
    if (m_commands.remove(id) == 0) {
        return false;
    }

    emit commandUnregistered(id);
    return true;
}

bool CommandRegistry::contains(const QString& id) const {
    return m_commands.contains(id);
}

CommandDescriptor CommandRegistry::command(const QString& id) const {
    return m_commands.value(id);
}

QVector<CommandDescriptor> CommandRegistry::commands() const {
    QVector<CommandDescriptor> result;
    result.reserve(m_commands.size());
    for (const auto& command : m_commands) {
        result.push_back(command);
    }
    return result;
}

int CommandRegistry::size() const {
    return m_commands.size();
}

} // namespace modernqgis
