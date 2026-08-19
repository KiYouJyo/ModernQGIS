// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <qgsmaptool.h>
#include <qgspointxy.h>

#include <QList>

class QKeyEvent;
class QgsMapCanvas;
class QgsMapMouseEvent;
class QgsMessageBar;
class QgsRubberBand;

namespace modernqgis {

class MeasureMapTool final : public QgsMapTool {
public:
    enum class Mode {
        Distance,
        Area,
    };

    MeasureMapTool(QgsMapCanvas* canvas, QgsMessageBar* messageBar, Mode mode);
    ~MeasureMapTool() override;

    void activate() override;
    void deactivate() override;
    void canvasMoveEvent(QgsMapMouseEvent* event) override;
    void canvasReleaseEvent(QgsMapMouseEvent* event) override;
    void canvasDoubleClickEvent(QgsMapMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Qgis::GeometryType geometryType() const;
    void rebuildBand(const QgsPointXY* previewPoint = nullptr);
    void resetMeasurement(bool clearBand = true);
    void finishMeasurement();

    QgsMessageBar* m_messageBar = nullptr;
    QgsRubberBand* m_band = nullptr;
    Mode m_mode = Mode::Distance;
    QList<QgsPointXY> m_points;
};

} // namespace modernqgis
