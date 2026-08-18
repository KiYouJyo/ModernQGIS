// SPDX-License-Identifier: GPL-2.0-or-later
#include "core/CommandRegistry.h"
#include <QSignalSpy>
#include <QtTest>

using namespace modernqgis;

class CommandRegistryTest final : public QObject {
    Q_OBJECT

private slots:
    void registersAndLooksUpCommand() {
        CommandRegistry registry;
        CommandDescriptor command;
        command.id = QStringLiteral("project.open");
        command.title = QStringLiteral("Open Project");
        command.category = QStringLiteral("project");

        QSignalSpy spy(&registry, &CommandRegistry::commandRegistered);

        QVERIFY(registry.registerCommand(command));
        QCOMPARE(registry.size(), 1);
        QVERIFY(registry.contains(command.id));
        QCOMPARE(registry.command(command.id).title, command.title);
        QCOMPARE(registry.command(command.id).category, command.category);
        QVERIFY(registry.command(command.id).action.isNull());
        QCOMPARE(spy.count(), 1);
    }

    void rejectsBlankAndDuplicateIds() {
        CommandRegistry registry;
        CommandDescriptor blank;
        QVERIFY(!registry.registerCommand(blank));

        CommandDescriptor command;
        command.id = QStringLiteral("map.pan");
        command.title = QStringLiteral("Pan");
        QVERIFY(registry.registerCommand(command));
        QVERIFY(!registry.registerCommand(command));
    }

    void unregistersCommand() {
        CommandRegistry registry;
        CommandDescriptor command;
        command.id = QStringLiteral("map.pan");
        command.title = QStringLiteral("Pan");
        QVERIFY(registry.registerCommand(command));
        QVERIFY(registry.unregisterCommand(command.id));
        QVERIFY(!registry.contains(command.id));
        QVERIFY(!registry.unregisterCommand(command.id));
    }
};

QTEST_GUILESS_MAIN(CommandRegistryTest)
#include "test_command_registry.moc"
