# Changelog

ModernQGIS uses Semantic Versioning for itself. QGIS compatibility is tracked separately.

## [Unreleased]

### Target: v0.3.0 — Figma-led Modern Shell

- Implemented the first reusable `ModernShellWindow` from the QGIS Modern Figma source of truth.
- Added code-level color, spacing and shell-geometry design tokens.
- Added the 40 px custom title bar, Ctrl+K global search, persistent Ribbon tabs and collapsible Ribbon workspace.
- Added Contents, document tabs, map workspace, Layer Properties and GIS status-bar primitives.
- Added pane/ribbon state persistence and baseline accessibility metadata.
- Added trilingual v0.3 shell resources for zh-CN, ja-JP and en-US.
- Added public `QgisInterface` wiring from shell actions to existing QGIS commands.
- Added an offscreen shell GUI smoke test and a dedicated QGIS 4.2 public-API integration build.

### Target: v0.2.0 — UI Abstraction Foundation

- Added Command, Pane and Context registries.
- Added bridge interfaces isolating QGIS integration from shell code.
- Added optional public-API QGIS QAction mappings.
- Added a standalone Qt 6 foundation sandbox.
- Added zh-CN, ja-JP and en-US resources and consistency checks.
- Added repository, documentation, CI and draft release contracts.

### Target: v0.1.0 — Architecture PoC

- Established the no-QGIS-fork repository boundary.
- Established Qt 6/CMake/C++20 foundations.
- Defined public-API-first, compatibility-layer and upstream-first rules.

> No v0.1.0, v0.2.0 or v0.3.0 tag is considered published until an immutable tag and GitHub Release actually exist.
