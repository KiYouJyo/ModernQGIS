// SPDX-License-Identifier: GPL-2.0-or-later
#include "bridge/QgisBridge.h"

#include "bridge/MeasureMapTool.h"
#include "core/CommandDescriptor.h"
#include "core/CommandRegistry.h"
#include "shell/ModernShellWindow.h"

#include <qgis.h>
#include <qgisinterface.h>
#include <qgsfeature.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgslayertreemodel.h>
#include <qgslayertreeview.h>
#include <qgsmapcanvas.h>
#include <qgsmapsettings.h>
#include <qgsmaptoolidentifyfeature.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>
#include <qgspointxy.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>
#include <qgswkbtypes.h>

#include <QAction>
#include <QDockWidget>
#include <QFileInfo>
#include <QMainWindow>
#include <QTimer>

namespace modernqgis {
namespace {

struct Binding {
    const char* id;
    const char* category;
    QAction* action;
};

QList<Binding> publicBindings(QgisInterface* iface) {
    if (!iface) return {};
    return {
        {"project.new", "project", iface->actionNewProject()},
        {"project.open", "project", iface->actionOpenProject()},
        {"project.save", "project", iface->actionSaveProject()},
        {"layer.attribute-table", "layer", iface->actionOpenTable()},
        {"layer.properties", "layer", iface->actionLayerProperties()},
        {"layer.toggle-editing", "edit", iface->actionToggleEditing()},
    };
}

QAction* registeredAction(CommandRegistry& registry, const QString& id) {
    if (!registry.contains(id)) return nullptr;
    return registry.command(id).action.data();
}

QString layerDetail(QgsMapLayer* layer) {
    if (auto* vector = qobject_cast<QgsVectorLayer*>(layer)) {
        const QString geometry = QgsWkbTypes::displayString(vector->wkbType());
        return QStringLiteral("%1 · %2 features").arg(geometry).arg(vector->featureCount());
    }
    if (auto* raster = qobject_cast<QgsRasterLayer*>(layer)) {
        return QStringLiteral("Raster · %1 bands").arg(raster->bandCount());
    }
    return layer ? QStringLiteral("Map layer") : QString{};
}

} // namespace

QgisBridge::QgisBridge(QgisInterface* iface, QObject* parent)
    : QObject(parent), m_iface(iface) {}

bool QgisBridge::isAvailable() const { return m_iface != nullptr; }

QString QgisBridge::qgisVersion() const {
    return m_iface ? Qgis::version() : QString{};
}

QMainWindow* QgisBridge::mainWindow() const {
    return m_iface ? qobject_cast<QMainWindow*>(m_iface->mainWindow()) : nullptr;
}

QList<QAction*> QgisBridge::discoverActions() const {
    const auto* window = mainWindow();
    return window ? window->findChildren<QAction*>() : QList<QAction*>{};
}

QList<QDockWidget*> QgisBridge::discoverDockWidgets() const {
    const auto* window = mainWindow();
    return window ? window->findChildren<QDockWidget*>() : QList<QDockWidget*>{};
}

int QgisBridge::seedCoreCommands(CommandRegistry& registry) const {
    int added = 0;
    for (const auto& binding : publicBindings(m_iface)) {
        if (!binding.action) continue;
        CommandDescriptor descriptor;
        descriptor.id = QString::fromLatin1(binding.id);
        descriptor.title = binding.action->text();
        descriptor.description = binding.action->toolTip();
        descriptor.category = QString::fromLatin1(binding.category);
        descriptor.action = binding.action;
        if (registry.registerCommand(descriptor)) ++added;
    }
    return added;
}

bool QgisBridge::attachWorkspace(ModernShellWindow* shell) {
    if (!m_iface || !shell) return false;
    if (m_canvas) return true;

    m_shell = shell;
    auto* project = QgsProject::instance();

    m_canvas = new QgsMapCanvas(shell);
    m_canvas->setProject(project);
    m_canvas->setCanvasColor(Qt::white);
    m_canvas->enableAntiAliasing(true);
    m_canvas->setParallelRenderingEnabled(true);
    m_canvas->setMinimumSize(420, 280);

    m_layerTreeModel = new QgsLayerTreeModel(project->layerTreeRoot(), this);
    m_layerTreeModel->setFlags(
        QgsLayerTreeModel::ShowLegend |
        QgsLayerTreeModel::UseTextFormatting |
        QgsLayerTreeModel::AllowNodeReorder |
        QgsLayerTreeModel::AllowNodeRename |
        QgsLayerTreeModel::AllowNodeChangeVisibility |
        QgsLayerTreeModel::AllowLegendChangeState |
        QgsLayerTreeModel::ActionHierarchical);

    m_layerTreeView = new QgsLayerTreeView(shell);
    m_layerTreeView->setModel(m_layerTreeModel);
    m_layerTreeView->setMessageBar(m_iface->messageBar());
    m_layerTreeView->setHeaderHidden(true);
    m_layerTreeView->setDragEnabled(true);
    m_layerTreeView->setAcceptDrops(true);
    m_layerTreeView->setDropIndicatorShown(true);

    m_layerCanvasBridge = new QgsLayerTreeMapCanvasBridge(project->layerTreeRoot(), m_canvas, this);

    shell->replaceMapCanvas(m_canvas);
    shell->replaceContentsTree(m_layerTreeView);

    const auto updateLayerSummary = [this](QgsMapLayer* layer) {
        if (!m_shell) return;
        m_shell->setActiveLayerSummary(layer ? layer->name() : QString{}, layerDetail(layer));
    };
    const auto updateMapStatus = [this](const QString& coordinates = QString{}, const QString& rendering = QString{}) {
        if (!m_shell || !m_canvas) return;
        const double scaleValue = m_canvas->scale();
        const double rotationValue = m_canvas->rotation();
        const QString scale = QStringLiteral("Scale 1:%1").arg(qRound64(scaleValue));
        const QString rotation = QStringLiteral("Rotation %1°").arg(rotationValue, 0, 'f', 1);
        const QString crs = m_canvas->mapSettings().destinationCrs().authid();
        m_shell->setMapStatus(coordinates, scale, rotation, crs,
                              rendering.isEmpty() ? QStringLiteral("✓ Rendering complete") : rendering);
    };

    m_panTool = new QgsMapToolPan(m_canvas);
    m_panTool->setParent(this);
    m_zoomInTool = new QgsMapToolZoom(m_canvas, false);
    m_zoomInTool->setParent(this);
    m_zoomOutTool = new QgsMapToolZoom(m_canvas, true);
    m_zoomOutTool->setParent(this);
    m_selectTool = new QgsMapToolIdentifyFeature(m_canvas);
    m_selectTool->setParent(this);
    m_measureDistanceTool = new MeasureMapTool(m_canvas, m_iface->messageBar(), MeasureMapTool::Mode::Distance);
    m_measureDistanceTool->setParent(this);
    m_measureAreaTool = new MeasureMapTool(m_canvas, m_iface->messageBar(), MeasureMapTool::Mode::Area);
    m_measureAreaTool->setParent(this);

    connect(m_selectTool,
            qOverload<const QgsFeature&>(&QgsMapToolIdentifyFeature::featureIdentified),
            this, [this](const QgsFeature& feature) {
        auto* layer = qobject_cast<QgsVectorLayer*>(m_iface ? m_iface->activeLayer() : nullptr);
        if (!layer) return;
        layer->selectByIds(QgsFeatureIds{feature.id()});
        if (m_canvas) m_canvas->refresh();
    });

    connect(m_layerTreeView, &QgsLayerTreeView::currentLayerChanged,
            this, [this, updateLayerSummary](QgsMapLayer* layer) {
        if (m_iface) m_iface->setActiveLayer(layer);
        if (m_selectTool) m_selectTool->setLayer(qobject_cast<QgsVectorLayer*>(layer));
        updateLayerSummary(layer);

        const bool vectorActive = qobject_cast<QgsVectorLayer*>(layer) != nullptr;
        if (m_shell) {
            if (auto* select = m_shell->commandAction(QStringLiteral("map.select"))) select->setEnabled(vectorActive);
            if (auto* clear = m_shell->commandAction(QStringLiteral("map.clear-selection"))) clear->setEnabled(vectorActive);
            if (auto* zoom = m_shell->commandAction(QStringLiteral("map.zoom-selection"))) zoom->setEnabled(vectorActive);
        }
    });

    connect(m_canvas, &QgsMapCanvas::xyCoordinates, this,
            [updateMapStatus](const QgsPointXY& point) {
        updateMapStatus(QStringLiteral("%1, %2").arg(point.x(), 0, 'f', 5).arg(point.y(), 0, 'f', 5));
    });
    connect(m_canvas, &QgsMapCanvas::extentsChanged, this,
            [updateMapStatus] { updateMapStatus(); });
    connect(m_canvas, &QgsMapCanvas::renderStarting, this,
            [updateMapStatus] { updateMapStatus(QString{}, QStringLiteral("Rendering…")); });
    connect(m_canvas, &QgsMapCanvas::mapCanvasRefreshed, this,
            [updateMapStatus] { updateMapStatus(QString{}, QStringLiteral("✓ Rendering complete")); });

    if (auto* active = m_iface->activeLayer()) {
        m_layerTreeView->setCurrentLayer(active);
        m_selectTool->setLayer(qobject_cast<QgsVectorLayer*>(active));
        updateLayerSummary(active);
    } else {
        updateLayerSummary(nullptr);
    }

    connect(m_iface, &QgisInterface::projectRead, this, [this, updateMapStatus] {
        syncProjectTitle();
        if (m_canvas) QTimer::singleShot(0, m_canvas, [this, updateMapStatus] {
            m_canvas->zoomToProjectExtent();
            updateMapStatus();
        });
    });
    connect(m_iface, &QgisInterface::newProjectCreated, this, [this, updateMapStatus] {
        syncProjectTitle();
        if (m_canvas) {
            m_canvas->refresh();
            updateMapStatus();
        }
    });

    syncProjectTitle();
    if (!project->mapLayers().isEmpty()) {
        QTimer::singleShot(0, m_canvas, [this, updateMapStatus] {
            m_canvas->zoomToProjectExtent();
            updateMapStatus();
        });
    } else {
        updateMapStatus();
    }
    m_canvas->setMapTool(m_panTool);
    return true;
}

int QgisBridge::bindRegisteredCommands(CommandRegistry& registry) {
    int bound = 0;

    // Commands which are safe to delegate directly to stable public
    // QgisInterface actions (project and active-layer dialogs/editing).
    for (const auto& binding : publicBindings(m_iface)) {
        if (!binding.action) continue;
        const auto id = QString::fromLatin1(binding.id);
        auto* shellAction = registeredAction(registry, id);
        if (!shellAction) continue;

        QAction* nativeAction = binding.action;
        connect(shellAction, &QAction::triggered, this, [nativeAction] {
            nativeAction->trigger();
        });
        shellAction->setEnabled(nativeAction->isEnabled());
        shellAction->setCheckable(nativeAction->isCheckable());
        shellAction->setChecked(nativeAction->isChecked());
        connect(nativeAction, &QAction::changed, shellAction, [shellAction, nativeAction] {
            shellAction->setEnabled(nativeAction->isEnabled());
            if (shellAction->isCheckable()) shellAction->setChecked(nativeAction->isChecked());
        });
        ++bound;
    }

    if (!m_canvas) return bound;

    const auto bindCanvas = [this, &registry, &bound](const QString& id, auto callback) {
        if (auto* action = registeredAction(registry, id)) {
            connect(action, &QAction::triggered, this, callback);
            ++bound;
        }
    };

    bindCanvas(QStringLiteral("map.explore"), [this] { m_canvas->setMapTool(m_panTool); });
    bindCanvas(QStringLiteral("map.pan"), [this] { m_canvas->setMapTool(m_panTool); });
    bindCanvas(QStringLiteral("map.zoom-in"), [this] { m_canvas->setMapTool(m_zoomInTool); });
    bindCanvas(QStringLiteral("map.zoom-out"), [this] { m_canvas->setMapTool(m_zoomOutTool); });
    bindCanvas(QStringLiteral("map.full-extent"), [this] { m_canvas->zoomToFullExtent(); });
    bindCanvas(QStringLiteral("map.select"), [this] {
        auto* layer = qobject_cast<QgsVectorLayer*>(m_iface ? m_iface->activeLayer() : nullptr);
        if (!layer) return;
        m_selectTool->setLayer(layer);
        m_canvas->setMapTool(m_selectTool);
    });
    bindCanvas(QStringLiteral("map.zoom-selection"), [this] {
        if (auto* layer = m_iface ? m_iface->activeLayer() : nullptr) m_canvas->zoomToSelected(layer);
    });
    bindCanvas(QStringLiteral("map.clear-selection"), [this] {
        if (auto* layer = qobject_cast<QgsVectorLayer*>(m_iface ? m_iface->activeLayer() : nullptr)) {
            layer->removeSelection();
            m_canvas->refresh();
        }
    });
    bindCanvas(QStringLiteral("map.measure"), [this] {
        if (m_measureDistanceTool) m_canvas->setMapTool(m_measureDistanceTool);
    });
    bindCanvas(QStringLiteral("map.measure-area"), [this] {
        if (m_measureAreaTool) m_canvas->setMapTool(m_measureAreaTool);
    });
    bindCanvas(QStringLiteral("layer.add-data"), [this] {
        if (m_iface) m_iface->openDataSourceManagerPage(QString());
    });

    return bound;
}

void QgisBridge::syncProjectTitle() const {
    if (!m_shell) return;
    const auto* project = QgsProject::instance();
    QString name = project->title().trimmed();
    if (name.isEmpty() && !project->fileName().isEmpty()) {
        name = QFileInfo(project->fileName()).fileName();
    }
    m_shell->setProjectDisplayName(name);
}

} // namespace modernqgis
