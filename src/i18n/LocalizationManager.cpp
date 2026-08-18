// SPDX-License-Identifier: GPL-2.0-or-later
#include "i18n/LocalizationManager.h"

#include <QCoreApplication>

namespace modernqgis {

LocalizationManager::LocalizationManager(QObject* parent) : QObject(parent) {}

QLocale LocalizationManager::normalizedLocale(const QLocale& requested) {
    if (requested.language() == QLocale::Chinese) return QLocale(QLocale::Chinese, QLocale::China);
    if (requested.language() == QLocale::Japanese) return QLocale(QLocale::Japanese, QLocale::Japan);
    return QLocale(QLocale::English, QLocale::UnitedStates);
}

bool LocalizationManager::apply(const QLocale& locale) {
    const auto normalized = normalizedLocale(locale);
    QCoreApplication::removeTranslator(&m_translator);
    const bool loaded = m_translator.load(QStringLiteral(":/i18n/modernqgis_%1.qm").arg(normalized.name()));
    if (loaded) QCoreApplication::installTranslator(&m_translator);
    m_activeLocale = normalized;
    QLocale::setDefault(normalized);
    emit localeChanged(normalized);
    return loaded;
}

QLocale LocalizationManager::activeLocale() const { return m_activeLocale; }

} // namespace modernqgis
