// SPDX-License-Identifier: GPL-2.0-or-later
#include "core/ContextRegistry.h"
#include <QSignalSpy>
#include <QtTest>
using namespace modernqgis;
class ContextRegistryTest final : public QObject {
    Q_OBJECT
private slots:
    void tracksActiveContexts() {
        ContextRegistry registry; QSignalSpy spy(&registry, &ContextRegistry::contextsChanged);
        QVERIFY(registry.activate(QStringLiteral("workspace.map"))); QVERIFY(registry.isActive(QStringLiteral("workspace.map"))); QVERIFY(!registry.activate(QStringLiteral("workspace.map"))); QCOMPARE(spy.count(), 1); QVERIFY(registry.deactivate(QStringLiteral("workspace.map"))); QVERIFY(!registry.isActive(QStringLiteral("workspace.map"))); QCOMPARE(spy.count(), 2);
    }
    void rejectsBlankContext() { ContextRegistry registry; QVERIFY(!registry.activate(QString{})); QVERIFY(!registry.activate(QStringLiteral("   "))); }
};
QTEST_GUILESS_MAIN(ContextRegistryTest)
#include "test_context_registry.moc"
