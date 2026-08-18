// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QObject>
#include <QSet>
#include <QStringList>

namespace modernqgis {

class ContextRegistry final : public QObject {
    Q_OBJECT

public:
    explicit ContextRegistry(QObject* parent = nullptr);

    bool activate(const QString& context);
    bool deactivate(const QString& context);
    bool isActive(const QString& context) const;
    QStringList activeContexts() const;

signals:
    void contextsChanged();

private:
    QSet<QString> m_activeContexts;
};

} // namespace modernqgis
