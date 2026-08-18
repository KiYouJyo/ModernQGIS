// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "core/CommandDescriptor.h"

#include <QHash>
#include <QObject>
#include <QVector>

namespace modernqgis {

class CommandRegistry final : public QObject {
    Q_OBJECT

public:
    explicit CommandRegistry(QObject* parent = nullptr);

    bool registerCommand(const CommandDescriptor& command);
    bool unregisterCommand(const QString& id);
    bool contains(const QString& id) const;
    CommandDescriptor command(const QString& id) const;
    QVector<CommandDescriptor> commands() const;
    int size() const;

signals:
    void commandRegistered(const QString& id);
    void commandUnregistered(const QString& id);

private:
    QHash<QString, CommandDescriptor> m_commands;
};

} // namespace modernqgis
