[简体中文](../../CONTRIBUTING.md) | [日本語](CONTRIBUTING.ja.md) | English

# Contributing

Use public QGIS/Qt APIs first, do not reimplement GIS behavior, isolate QGIS compatibility in bridge/compat code, maintain zh-CN/ja-JP/en-US user-facing strings, and never commit credentials or signing material.

Before merge, build, test, run `scripts/check_i18n.py` and `scripts/check_release_contract.py`, and ensure new user-visible copy has all three translations. Formal maintainer commits/tags should use the configured signing policy when performed from a signing-capable environment; automation must not pretend an unsigned commit is Verified.
