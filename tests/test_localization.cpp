// SPDX-License-Identifier: GPL-2.0-or-later
#include "i18n/LocalizationManager.h"
#include <QtTest>
using namespace modernqgis;
class LocalizationTest final : public QObject {
    Q_OBJECT
private slots:
    void normalizesSupportedLanguages() {
        QCOMPARE(LocalizationManager::normalizedLocale(QLocale(QLocale::Chinese, QLocale::Taiwan)).name(), QStringLiteral("zh_CN"));
        QCOMPARE(LocalizationManager::normalizedLocale(QLocale(QLocale::Japanese, QLocale::Japan)).name(), QStringLiteral("ja_JP"));
        QCOMPARE(LocalizationManager::normalizedLocale(QLocale(QLocale::German, QLocale::Germany)).name(), QStringLiteral("en_US"));
    }
};
QTEST_GUILESS_MAIN(LocalizationTest)
#include "test_localization.moc"
