// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <qgisplugin.h>
#include <QObject>

class QAction;
class QgisInterface;

namespace modernqgis {
class ModernQgisPlugin final : public QObject, public QgisPlugin {
    Q_OBJECT
public:
    explicit ModernQgisPlugin(QgisInterface* iface);
    ~ModernQgisPlugin() override;
    void initGui() override;
    void unload() override;
private:
    QgisInterface* m_iface = nullptr;
    QAction* m_action = nullptr;
};
} // namespace modernqgis
