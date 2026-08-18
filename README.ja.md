[简体中文](README.md) | 日本語 | [English](README.en.md)

# ModernQGIS

QGIS 向けのモダンなデスクトップ Shell と操作アーキテクチャを検証するプロジェクトです。

> **現在：v0.1–v0.2 Foundation。** アーキテクチャ、QGIS bridge、Command/Pane/Context 抽象化、多言語化、リリース基盤を整備する段階です。日常利用向けの完成 UI ではありません。

ModernQGIS は独自 GIS エンジンを持たず、QGIS ソースツリーも同梱しません。QGIS 固有差分を bridge/compat 層へ隔離し、将来 upstream へ還元可能な汎用 UI 基盤を目指します。

## Foundation

- Qt 6 + CMake + C++20。
- Command / Pane / Context Registry。
- `IQgisBridge` と公開 API ベースの任意 QGIS bridge。
- Project/Data/Navigation/Identify/Measure/Editing の初期 QAction マッピング。
- `modernqgis-sandbox`。
- zh-CN / ja-JP / en-US 三言語リソースと自動整合性検査。
- CI、Release 契約、文章規約、ロードマップ、upstream 方針。

```bash
cmake -S . -B build -DMODERNQGIS_WITH_QGIS=OFF
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Preview package は `ModernQGIS-ShellPreview-vX.Y.Z-<platform>-<arch>.zip` + `SHA256SUMS` とし、QGIS 本体は同梱しません。

ModernQGIS は独立したコミュニティ実験であり、現時点では **QGIS.org / QGIS Project の公式製品ではありません**。GPL-2.0-or-later で公開します。
