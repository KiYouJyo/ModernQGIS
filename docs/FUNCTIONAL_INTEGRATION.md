# Functional integration roadmap

This document tracks the transition from the v0.3 presentation shell to a functional QGIS-backed workspace.

## v0.4 first usable GIS loop

- real `QgsMapCanvas` injected through the QGIS bridge
- real `QgsLayerTreeView` backed by the active `QgsProject`
- project/layer actions connected through public `QgisInterface` APIs
- shell map navigation bound to the embedded canvas
- active-layer synchronization between the ModernQGIS contents tree and QGIS
- project title and canvas status synchronized into the shell

## Architectural rule

QGIS-specific classes remain in `src/bridge` and `src/plugin`. The reusable shell exposes generic widget/action attachment points only. This keeps the presentation layer independent from QGIS API churn.
