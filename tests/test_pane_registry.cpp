// SPDX-License-Identifier: GPL-2.0-or-later
#include "core/PaneRegistry.h"
#include <QSignalSpy>
#include <QtTest>
using namespace modernqgis;
class PaneRegistryTest final : public QObject {
    Q_OBJECT
private slots:
    void registersPaneMetadata() {
        PaneRegistry registry; PaneDescriptor pane; pane.id = QStringLiteral("contents"); pane.title = QStringLiteral("Contents"); pane.preferredLocation = PaneLocation::Left;
        QSignalSpy spy(&registry, &PaneRegistry::paneRegistered); QVERIFY(registry.registerPane(pane)); QCOMPARE(registry.size(), 1); QVERIFY(registry.contains(pane.id)); QCOMPARE(registry.pane(pane.id).preferredLocation, PaneLocation::Left); QCOMPARE(spy.count(), 1);
    }
    void rejectsBlankAndDuplicateIds() {
        PaneRegistry registry; PaneDescriptor blank; QVERIFY(!registry.registerPane(blank)); PaneDescriptor pane; pane.id = QStringLiteral("properties"); pane.title = QStringLiteral("Properties"); QVERIFY(registry.registerPane(pane)); QVERIFY(!registry.registerPane(pane));
    }
};
QTEST_GUILESS_MAIN(PaneRegistryTest)
#include "test_pane_registry.moc"
