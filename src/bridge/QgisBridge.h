// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "bridge/IQgisBridge.h"

#include <QObject>

class QgisInterface;
class QgsLayerTreeMapCanvasBridge;
class QgsLayerTreeModel;
class QgsLayerTreeView;
class QgsMapCanvas;
class QgsMapToolIdentifyFeature;
class QgsMapToolPan;
class QgsMapToolZoom;

namespace modernqgis {

class MeasureMapTool;
class ModernShellWindow;

class QgisBridge final : public QObject, public IQgisBridge {
public:
    explicit QgisBridge(QgisInterface* iface, QObject* parent = nullptr);

    bool isAvailable() const override;
    QString qgisVersion() const override;
    QMainWindow* mainWindow() const override;
    QList<QAction*> discoverActions() const override;
    QList<QDockWidget*> discoverDockWidgets() const override;
    int seedCoreCommands(CommandRegistry& registry) const override;

    // Replaces shell preview widgets with a real QGIS-backed map canvas and
    // layer tree while keeping QGIS API types out of the presentation layer.
    bool attachWorkspace(ModernShellWindow* shell);

    // Connects ModernQGIS presentation actions to public QGIS APIs and to the
    // embedded map canvas created by attachWorkspace().
    int bindRegisteredCommands(CommandRegistry& registry);

private:
    void syncProjectTitle() const;

    QgisInterface* m_iface = nullptr;
    ModernShellWindow* m_shell = nullptr;
    QgsMapCanvas* m_canvas = nullptr;
    QgsLayerTreeView* m_layerTreeView = nullptr;
    QgsLayerTreeModel* m_layerTreeModel = nullptr;
    QgsLayerTreeMapCanvasBridge* m_layerCanvasBridge = nullptr;
    QgsMapToolPan* m_panTool = nullptr;
    QgsMapToolZoom* m_zoomInTool = nullptr;
    QgsMapToolZoom* m_zoomOutTool = nullptr;
    QgsMapToolIdentifyFeature* m_selectTool = nullptr;
    MeasureMapTool* m_measureDistanceTool = nullptr;
    MeasureMapTool* m_measureAreaTool = nullptr;
};

} // namespace modernqgis
