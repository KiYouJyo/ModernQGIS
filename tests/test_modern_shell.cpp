// SPDX-License-Identifier: GPL-2.0-or-later
#include "shell/ModernShellWindow.h"
#include "shell/ShellStyle.h"
#include <QtTest>
#include <QWidget>

using namespace modernqgis;

class ModernShellTest final : public QObject {
    Q_OBJECT
private slots:
    void exposesFigmaReferenceGeometry() {
        ModernShellWindow window;
        QCOMPARE(window.minimumWidth(), shellstyle::MinimumWindowWidth);
        QCOMPARE(window.minimumHeight(), shellstyle::MinimumWindowHeight);
        QVERIFY(window.commandRegistry().contains(QStringLiteral("map.explore")));
        QVERIFY(window.commandRegistry().contains(QStringLiteral("layer.attribute-table")));
        QVERIFY(window.paneRegistry().contains(QStringLiteral("contents")));
        QVERIFY(window.paneRegistry().contains(QStringLiteral("properties")));
        QVERIFY(window.contextRegistry().isActive(QStringLiteral("workspace.map")));
    }

    void exposesPresentationNeutralIntegrationPoints() {
        ModernShellWindow window;
        QVERIFY(window.commandAction(QStringLiteral("map.pan")) != nullptr);

        auto* map = new QWidget;
        map->setObjectName(QStringLiteral("InjectedMapCanvas"));
        window.replaceMapCanvas(map);
        QVERIFY(window.findChild<QWidget*>(QStringLiteral("InjectedMapCanvas")) == map);

        auto* tree = new QWidget;
        tree->setObjectName(QStringLiteral("InjectedContentsTree"));
        window.replaceContentsTree(tree);
        QVERIFY(window.findChild<QWidget*>(QStringLiteral("InjectedContentsTree")) == tree);

        window.setProjectDisplayName(QStringLiteral("functional.qgz"));
        QCOMPARE(window.windowTitle(), QStringLiteral("ModernQGIS · functional.qgz"));
    }
};

QTEST_MAIN(ModernShellTest)
#include "test_modern_shell.moc"
