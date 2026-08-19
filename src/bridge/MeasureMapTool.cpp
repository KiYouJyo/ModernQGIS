// SPDX-License-Identifier: GPL-2.0-or-later
#include "bridge/MeasureMapTool.h"

#include <qgsdistancearea.h>
#include <qgsmapcanvas.h>
#include <qgsmapmouseevent.h>
#include <qgsmapsettings.h>
#include <qgsmessagebar.h>
#include <qgsproject.h>
#include <qgsrubberband.h>
#include <qgsunittypes.h>

#include <QColor>
#include <QKeyEvent>
#include <QtMath>

namespace modernqgis {

MeasureMapTool::MeasureMapTool(QgsMapCanvas* canvas, QgsMessageBar* messageBar, Mode mode)
    : QgsMapTool(canvas), m_messageBar(messageBar), m_mode(mode) {
    m_band = new QgsRubberBand(canvas, geometryType());
    m_band->setStrokeColor(QColor(46, 135, 92));
    m_band->setSecondaryStrokeColor(QColor(255, 255, 255, 210));
    m_band->setWidth(2);
    if (m_mode == Mode::Area) {
        m_band->setFillColor(QColor(46, 135, 92, 42));
    }
    setCursor(Qt::CrossCursor);
    setToolName(m_mode == Mode::Distance ? QStringLiteral("ModernQGIS Distance")
                                         : QStringLiteral("ModernQGIS Area"));
}

MeasureMapTool::~MeasureMapTool() {
    delete m_band;
    m_band = nullptr;
}

Qgis::GeometryType MeasureMapTool::geometryType() const {
    return m_mode == Mode::Area ? Qgis::GeometryType::Polygon : Qgis::GeometryType::Line;
}

void MeasureMapTool::activate() {
    QgsMapTool::activate();
    resetMeasurement();
    if (m_messageBar) {
        const QString hint = m_mode == Mode::Distance
            ? QObject::tr("Distance: left-click to add points; right-click or Enter to finish; Esc clears.")
            : QObject::tr("Area: left-click to add vertices; right-click or Enter to finish; Esc clears.");
        m_messageBar->pushInfo(QObject::tr("ModernQGIS Measure"), hint);
    }
}

void MeasureMapTool::deactivate() {
    resetMeasurement();
    QgsMapTool::deactivate();
}

void MeasureMapTool::canvasMoveEvent(QgsMapMouseEvent* event) {
    if (!event || m_points.isEmpty()) return;
    const QgsPointXY preview = event->snapPoint();
    rebuildBand(&preview);
}

void MeasureMapTool::canvasReleaseEvent(QgsMapMouseEvent* event) {
    if (!event) return;
    if (event->button() == Qt::RightButton) {
        finishMeasurement();
        return;
    }
    if (event->button() != Qt::LeftButton) return;

    if (m_points.isEmpty() && m_band) m_band->reset(geometryType());
    m_points.append(event->snapPoint());
    rebuildBand();
}

void MeasureMapTool::canvasDoubleClickEvent(QgsMapMouseEvent* event) {
    if (event && event->button() == Qt::LeftButton) finishMeasurement();
}

void MeasureMapTool::keyPressEvent(QKeyEvent* event) {
    if (!event) return;
    if (event->key() == Qt::Key_Escape) {
        resetMeasurement();
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        finishMeasurement();
        event->accept();
        return;
    }
    QgsMapTool::keyPressEvent(event);
}

void MeasureMapTool::rebuildBand(const QgsPointXY* previewPoint) {
    if (!m_band) return;
    m_band->reset(geometryType());
    for (const QgsPointXY& point : m_points) m_band->addPoint(point, false);
    if (previewPoint) m_band->addPoint(*previewPoint, false);
    m_band->updatePosition();
}

void MeasureMapTool::resetMeasurement(bool clearBand) {
    m_points.clear();
    if (clearBand && m_band) m_band->reset(geometryType());
}

void MeasureMapTool::finishMeasurement() {
    const int minimumPoints = m_mode == Mode::Area ? 3 : 2;
    if (m_points.size() < minimumPoints) {
        if (m_messageBar) {
            m_messageBar->pushInfo(QObject::tr("ModernQGIS Measure"),
                                   m_mode == Mode::Area
                                       ? QObject::tr("Add at least three vertices to measure area.")
                                       : QObject::tr("Add at least two points to measure distance."));
        }
        return;
    }

    QgsDistanceArea measure;
    if (canvas()) {
        measure.setSourceCrs(canvas()->mapSettings().destinationCrs(), QgsProject::instance()->transformContext());
    }
    const QString ellipsoid = QgsProject::instance()->ellipsoid();
    if (!ellipsoid.isEmpty() && ellipsoid.compare(QStringLiteral("NONE"), Qt::CaseInsensitive) != 0) {
        measure.setEllipsoid(ellipsoid);
    }

    QString result;
    if (m_mode == Mode::Area) {
        const double value = qAbs(measure.measurePolygon(m_points));
        const auto scaled = QgsUnitTypes::scaledArea(value, measure.areaUnits(), 3, false);
        result = QObject::tr("Area: %1 %2")
            .arg(scaled.value, 0, 'f', 3)
            .arg(QgsUnitTypes::toAbbreviatedString(scaled.unit));
        if (m_band) m_band->closePoints(true);
    } else {
        double value = 0.0;
        for (int i = 1; i < m_points.size(); ++i) {
            value += measure.measureLine(m_points.at(i - 1), m_points.at(i));
        }
        const auto scaled = QgsUnitTypes::scaledDistance(value, measure.lengthUnits(), 3, false);
        result = QObject::tr("Distance: %1 %2")
            .arg(scaled.value, 0, 'f', 3)
            .arg(QgsUnitTypes::toAbbreviatedString(scaled.unit));
    }

    if (m_messageBar) m_messageBar->pushInfo(QObject::tr("ModernQGIS Measure"), result);
    m_points.clear();
}

} // namespace modernqgis
