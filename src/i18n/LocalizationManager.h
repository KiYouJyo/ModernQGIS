// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QLocale>
#include <QObject>
#include <QTranslator>

namespace modernqgis {

class LocalizationManager final : public QObject {
    Q_OBJECT
public:
    explicit LocalizationManager(QObject* parent = nullptr);
    bool apply(const QLocale& locale);
    QLocale activeLocale() const;
    static QLocale normalizedLocale(const QLocale& requested);
signals:
    void localeChanged(const QLocale& locale);
private:
    QTranslator m_translator;
    QLocale m_activeLocale = QLocale(QLocale::English, QLocale::UnitedStates);
};

} // namespace modernqgis
