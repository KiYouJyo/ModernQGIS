[简体中文](README.md) | [日本語](README.ja.md) | English

# ModernQGIS

A modern desktop shell and interaction-architecture experiment for QGIS.

> **Current status: v0.1–v0.2 Foundation.** This stage establishes architecture, QGIS bridging, command/pane/context abstractions, localization and release infrastructure. It is not yet a daily-use replacement UI for QGIS.

ModernQGIS does not maintain a second GIS engine or vendor the QGIS source tree. QGIS-specific differences stay behind bridge/compatibility boundaries so generic shell infrastructure can evolve independently and remain suitable for future upstream work.

## Foundation

- Qt 6 + CMake + C++20 project skeleton.
- Command, Pane and Context registries.
- `IQgisBridge` and an optional public-API QGIS bridge.
- Initial native QAction bindings for project, data, navigation, identify, measure and editing.
- Standalone `modernqgis-sandbox`.
- zh-CN / ja-JP / en-US resources and automated parity checks.
- CI, release contract, text standards, roadmap and upstream strategy.

## Build

```bash
cmake -S . -B build -DMODERNQGIS_WITH_QGIS=OFF
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Optional QGIS integration:

```bash
cmake -S . -B build-qgis -DMODERNQGIS_WITH_QGIS=ON -DQGIS_PREFIX_PATH=/path/to/qgis/install
cmake --build build-qgis --config Release
```

Preview packages are named `ModernQGIS-ShellPreview-vX.Y.Z-<platform>-<arch>.zip` with `SHA256SUMS`, and do not bundle QGIS.

ModernQGIS is an independent community experiment and is currently **not an official product of QGIS.org or the QGIS Project**. Licensed under GPL-2.0-or-later.
