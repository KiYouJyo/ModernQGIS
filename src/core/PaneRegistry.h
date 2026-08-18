// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "core/PaneDescriptor.h"

#include <QHash>
#include <QObject>
#include <QVector>

namespace modernqgis {

class PaneRegistry final : public QObject {
    Q_OBJECT

public:
    explicit PaneRegistry(QObject* parent = nullptr);

    bool registerPane(const PaneDescriptor& pane);
    bool unregisterPane(const QString& id);
    bool contains(const QString& id) const;
    PaneDescriptor pane(const QString& id) const;
    QVector<PaneDescriptor> panes() const;
    int size() const;

signals:
    void paneRegistered(const QString& id);
    void paneUnregistered(const QString& id);

private:
    QHash<QString, PaneDescriptor> m_panes;
};

} // namespace modernqgis
