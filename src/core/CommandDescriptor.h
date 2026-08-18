// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QPointer>
#include <QString>
#include <QStringList>

class QAction;

namespace modernqgis {

struct CommandDescriptor {
    QString id;
    QString title;
    QString description;
    QString category;
    QStringList contexts;
    QPointer<QAction> action;
};

} // namespace modernqgis
