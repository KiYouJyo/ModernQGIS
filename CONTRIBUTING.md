简体中文 | English / 日本語 summaries are maintained in project documentation.

# 参与 ModernQGIS 开发

## 核心规则

1. **Public API first**：优先使用 QGIS/Qt 公开 API。
2. **No GIS reimplementation**：QGIS 已有 GIS 行为由原生 QAction/API/Processing 执行。
3. **Compatibility isolation**：QGIS 版本差异只进入 bridge/compat 层。
4. **Capability over version**：优先能力检测，而非写死小版本。
5. **Three-language contract**：用户可见文本必须维护简体中文、日本語、English。
6. **No secrets**：证书、私钥、token、个人路径和本地环境文件禁止提交。

分支使用 `agent/<goal>`、`feature/<goal>`、`fix/<goal>`。正式维护提交/tag 在具备签名能力的环境中按维护者签名策略执行；自动/API 生成提交不得伪称 Verified。

PR 合并前至少要求：build/test、`scripts/check_i18n.py`、`scripts/check_release_contract.py` 全部通过，新增用户文本三语齐全，不提交 QGIS 二进制与签名材料。

版本开发遵守 Goal 机制：**全部修复与验收条件完成后才进入 Release/PR 收束。**
