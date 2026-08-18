# ModernQGIS Architecture

## Objective

ModernQGIS modernizes presentation and interaction without creating a second GIS implementation or a long-lived modified QGIS source tree.

```text
shell / workspace (v0.3+)
        │
Command / Pane / Context registries
        │
IQgisBridge
        │
bridge / compat
        │
QGIS public API → QGIS
```

## Rules

- **No GIS reimplementation**: rendering, editing, project persistence, providers and Processing remain QGIS responsibilities.
- **Public API first**: private headers, child-order assumptions and object-name scraping must not become normal integration contracts.
- **Compatibility isolation**: QGIS-specific differences stay in `bridge/` or future `compat/` code.
- **Capability over version**: prefer capability detection to hard-coded QGIS minor versions.
- **Classic-safe development**: pre-1.0 work must not destructively remove the classic QGIS recovery path.

`CommandRegistry`, `PaneRegistry`, and `ContextRegistry` deliberately know nothing about Ribbon visuals. `QgisBridge` currently exposes explicit public-action bindings plus generic QAction/QDockWidget discovery for PoC diagnostics. Discovery is not permission to depend permanently on private child ordering/object names.

For each new infrastructure change ask: **if the ModernQGIS visual shell disappeared, would this abstraction still improve QGIS desktop architecture?** If yes, keep it presentation-neutral and consider it an upstream candidate.
