// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QPointer>
#include <QString>

class QWidget;

namespace modernqgis {

enum class PaneLocation {
    Left,
    Right,
    Bottom,
    Floating
};

struct PaneDescriptor {
    QString id;
    QString title;
    PaneLocation preferredLocation = PaneLocation::Right;
    QPointer<QWidget> widget;
};

} // namespace modernqgis
