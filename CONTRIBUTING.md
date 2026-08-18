简体中文 | [日本語](docs/i18n/CONTRIBUTING.ja.md) | [English](docs/i18n/CONTRIBUTING.en.md)

# 参与 ModernQGIS 开发

ModernQGIS 的长期目标不是维护一个改得越来越深的 QGIS fork，而是建立可独立演进、可跟随 QGIS 更新、并能把通用基础设施逐步贡献回 upstream 的现代桌面 Shell。

## 基本原则

1. **Public API first**：优先使用 QGIS/Qt 的公开 API；依赖私有实现必须有明确的兼容层、说明和测试。
2. **No GIS reimplementation**：QGIS 已有的 GIS 行为由原生 QAction/API/Processing 等能力执行，ModernQGIS 只组织与呈现。
3. **Compatibility isolation**：版本差异只进入 `src/compat` 或 bridge 层，Shell 与 Registry 不允许散落版本判断。
4. **Capability over version**：能通过能力检测解决的问题，不写死 QGIS 小版本号。
5. **Three-language contract**：用户可见文本必须进入翻译资源；简体中文、日本語、English 三套资源必须保持键集合一致。
6. **No secrets**：证书、私钥、token、个人路径和本地环境文件禁止提交。

## 分支与提交

- `main`：可构建、可测试的集成分支。
- 开发分支：`agent/<goal>`、`feature/<goal>`、`fix/<goal>`。
- 提交建议使用简短祈使句，并保持一次提交解决一个可描述的问题。
- 维护者本地环境具备签名能力时，正式维护提交与 tag 应使用已配置签名；自动化或 API 生成提交不得伪称为已签名。

## Pull Request 验收

PR 合并前至少要求：

- CMake configure/build 通过；
- 单元测试通过；
- `scripts/check_i18n.py` 通过；
- `scripts/check_release_contract.py` 通过；
- 新增公共接口有测试；
- 用户可见文本已三语覆盖；
- 不提交 QGIS 二进制、私钥、证书或本机构建产物。

版本开发应遵守 Goal 机制：**全部修复与验收条件完成后，才进入 Release/PR 收束**。

## 代码风格

C++ 使用 C++20、4 空格缩进、100 列软限制，提交前可运行 `clang-format`。SPDX 标识统一使用：

```text
SPDX-License-Identifier: GPL-2.0-or-later
```

详细架构、文本和发布规范见 `docs/`。
