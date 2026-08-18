简体中文 | [日本語](README.ja.md) | [English](README.en.md)

# ModernQGIS

面向 QGIS 的现代桌面 Shell 与交互架构实验项目。

[![CI](https://github.com/KiYouJyo/ModernQGIS/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/KiYouJyo/ModernQGIS/actions/workflows/ci.yml) [![License](https://img.shields.io/badge/License-GPL--2.0--or--later-D4A72C)](LICENSE) [![Qt](https://img.shields.io/badge/Qt-6.6%2B-41CD52?logo=qt&logoColor=white)](https://www.qt.io/) [![Languages](https://img.shields.io/badge/Languages-%E4%B8%AD%E6%96%87%20%7C%20%E6%97%A5%E6%9C%AC%E8%AA%9E%20%7C%20English-6F42C1)](docs/LOCALIZATION.md)

> **当前状态：v0.1–v0.2 Foundation。** 目前重点是架构、QGIS 桥接、命令/面板/上下文抽象、三语和发布基础，不是可替代 QGIS 日常使用的完整 UI。

ModernQGIS 不维护独立 GIS 内核，也不把 QGIS 源码复制进本仓库。目标是在 QGIS 的公开能力之上建立可替换的现代 UI 层，让未来 QGIS 更新尽量只影响 bridge/compat 层，并为通用基础设施逐步进入 QGIS upstream 保留路径。

## v0.1–v0.2 基础

- Qt 6 + CMake + C++20 独立工程骨架。
- `CommandRegistry`、`PaneRegistry`、`ContextRegistry`。
- `IQgisBridge` / `QgisBridge`：隔离 QGIS 依赖。
- 首批 QGIS 原生 QAction 映射：项目、新增数据、地图浏览、识别、量测、编辑。
- `modernqgis-sandbox`：不安装 QGIS 也可验证 Shell 基础结构。
- 简体中文、日本語、English 三语资源与一致性检查。
- CI、发布合同、文本规范、路线图与 upstream 策略。

## 架构

```text
ModernQGIS Shell / future Ribbon / Workspace / Panes
                         │
            Command / Pane / Context Registry
                         │
                    IQgisBridge
                         │
       QGIS public APIs / QAction / QgisInterface
                         │
                       QGIS
```

**不复制 QGIS 的 GIS 业务逻辑。** 公开 API 不足时，优先在 bridge/compat 层做最小处理，并把通用 API 缺口作为未来 upstream 候选。

## 构建 Foundation Sandbox

依赖 CMake 3.22+、C++20 编译器、Qt 6.6+。

```bash
cmake -S . -B build -DMODERNQGIS_WITH_QGIS=OFF
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## 可选 QGIS Bridge

```bash
cmake -S . -B build-qgis -DMODERNQGIS_WITH_QGIS=ON -DQGIS_PREFIX_PATH=/path/to/qgis/install
cmake --build build-qgis --config Release
```

本仓库不会下载或 vendor QGIS 本体。

## 三语与 Release

UI 与核心项目文档维护 `zh-CN`、`ja-JP`、`en-US`。运行 `python scripts/check_i18n.py` 检查三语一致性。

Foundation 预览包约定：

```text
ModernQGIS-ShellPreview-vX.Y.Z-<platform>-<arch>.zip
SHA256SUMS
```

预览 ZIP **不捆绑 QGIS**，也不冒充 QGIS 官方发行版。详细规则见 [发布合同](docs/RELEASE.md)。

## 文档

[架构](docs/ARCHITECTURE.md) · [路线图](docs/ROADMAP.md) · [本地化](docs/LOCALIZATION.md) · [文本规范](docs/TEXT_STYLE.md) · [发布合同](docs/RELEASE.md) · [Upstream](docs/UPSTREAM.md) · [Changelog](CHANGELOG.md)

## License 与项目身份

ModernQGIS 使用 GNU GPL v2 或更高版本。ModernQGIS 是独立社区实验项目，目前**不是 QGIS.org、QGIS Project 或其治理机构发布的官方 QGIS 产品**。
