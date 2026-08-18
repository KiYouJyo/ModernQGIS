// SPDX-License-Identifier: GPL-2.0-or-later
#include "i18n/LocalizationManager.h"
#include "sandbox/SandboxWindow.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("ModernQGIS"));
    QApplication::setApplicationVersion(QStringLiteral(MODERNQGIS_VERSION));
    QApplication::setOrganizationName(QStringLiteral("ModernQGIS"));
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("ModernQGIS v0.3 shell preview"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption languageOption({QStringLiteral("l"), QStringLiteral("lang")},
        QStringLiteral("UI language: zh_CN, ja_JP, or en_US"), QStringLiteral("locale"));
    parser.addOption(languageOption);
    parser.process(app);
    modernqgis::LocalizationManager localization;
    const auto requested = parser.isSet(languageOption) ? QLocale(parser.value(languageOption)) : QLocale::system();
    localization.apply(requested);
    modernqgis::SandboxWindow window;
    window.show();
    return app.exec();
}
