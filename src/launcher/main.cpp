// SPDX-License-Identifier: GPL-2.0-or-later
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QProcessEnvironment>
#include <QStandardPaths>

namespace {

QString firstExisting(const QStringList& candidates) {
    for (const QString& candidate : candidates) {
        if (!candidate.isEmpty() && QFileInfo::exists(candidate)) return QDir::toNativeSeparators(candidate);
    }
    return {};
}

QString findQgisExecutable() {
    const QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    const QString explicitPath = env.value(QStringLiteral("MODERNQGIS_QGIS_EXECUTABLE"));
    if (QFileInfo::exists(explicitPath)) return QDir::toNativeSeparators(explicitPath);

    QStringList candidates;
    const QString prefix = env.value(QStringLiteral("QGIS_PREFIX_PATH"));
    if (!prefix.isEmpty()) {
        candidates << QDir(prefix).filePath(QStringLiteral("bin/qgis-bin.exe"))
                   << QDir(prefix).filePath(QStringLiteral("bin/qgis.exe"))
                   << QDir(prefix).filePath(QStringLiteral("qgis-bin.exe"))
                   << QDir(prefix).filePath(QStringLiteral("qgis.exe"));
    }

#ifdef Q_OS_WIN
    candidates << QStringLiteral("C:/OSGeo4W/bin/qgis-bin.exe")
               << QStringLiteral("C:/OSGeo4W/bin/qgis.exe");

    const QDir programFiles(QStringLiteral("C:/Program Files"));
    const QStringList qgisDirs = programFiles.entryList({QStringLiteral("QGIS*")}, QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name | QDir::Reversed);
    for (const QString& dirName : qgisDirs) {
        const QString root = programFiles.filePath(dirName);
        candidates << QDir(root).filePath(QStringLiteral("bin/qgis-bin.exe"))
                   << QDir(root).filePath(QStringLiteral("bin/qgis.exe"))
                   << QDir(root).filePath(QStringLiteral("apps/qgis/bin/qgis-bin.exe"))
                   << QDir(root).filePath(QStringLiteral("apps/qgis/bin/qgis.exe"));
    }
#else
    candidates << QStringLiteral("/usr/bin/qgis") << QStringLiteral("/usr/local/bin/qgis");
#endif

    const QString discovered = firstExisting(candidates);
    if (!discovered.isEmpty()) return discovered;

    QString onPath = QStandardPaths::findExecutable(QStringLiteral("qgis-bin"));
    if (onPath.isEmpty()) onPath = QStandardPaths::findExecutable(QStringLiteral("qgis"));
    return onPath;
}

} // namespace

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName(QStringLiteral("ModernQGIS"));
    QApplication::setApplicationName(QStringLiteral("ModernQGIS Launcher"));

    const QString qgis = findQgisExecutable();
    if (qgis.isEmpty()) {
        QMessageBox::critical(nullptr,
                              QObject::tr("ModernQGIS"),
                              QObject::tr("QGIS 4.x was not found. Install QGIS 4.2 or set MODERNQGIS_QGIS_EXECUTABLE to qgis-bin.exe."));
        return 2;
    }

    const QString pluginPath = QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("plugins"));
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    const QString oldPluginPath = environment.value(QStringLiteral("QGIS_PLUGINPATH"));
    const QChar separator = QDir::listSeparator();
    environment.insert(QStringLiteral("QGIS_PLUGINPATH"),
                       oldPluginPath.isEmpty() ? pluginPath : pluginPath + separator + oldPluginPath);
    environment.insert(QStringLiteral("MODERNQGIS_AUTO_OPEN"), QStringLiteral("1"));

    QProcess process;
    process.setProgram(qgis);
    process.setProcessEnvironment(environment);
    process.setWorkingDirectory(QFileInfo(qgis).absolutePath());
    process.setArguments(QCoreApplication::arguments().mid(1));

    qint64 pid = 0;
    if (!process.startDetached(&pid)) {
        QMessageBox::critical(nullptr,
                              QObject::tr("ModernQGIS"),
                              QObject::tr("QGIS was found, but ModernQGIS could not start it."));
        return 3;
    }
    return 0;
}
