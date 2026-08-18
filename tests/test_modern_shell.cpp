// SPDX-License-Identifier: GPL-2.0-or-later
#include "shell/ModernShellWindow.h"
#include "shell/ShellStyle.h"
#include <QtTest>

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
};

QTEST_MAIN(ModernShellTest)
#include "test_modern_shell.moc"
