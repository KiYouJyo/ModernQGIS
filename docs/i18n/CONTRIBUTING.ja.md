[简体中文](../../CONTRIBUTING.md) | 日本語 | [English](CONTRIBUTING.en.md)

# コントリビューション

QGIS/Qt の公開 API を優先し、GIS 機能を再実装せず、QGIS 互換処理を bridge/compat 層へ隔離してください。ユーザー向け文字列は zh-CN / ja-JP / en-US の三言語を維持し、認証情報・秘密鍵・署名素材はコミットしません。

マージ前に build/test、`scripts/check_i18n.py`、`scripts/check_release_contract.py` を実行し、新しいユーザー向け文言に三言語翻訳があることを確認します。署名可能な環境で行う正式な maintainer commit/tag は設定済みポリシーに従い、未署名の自動/API commit を Verified と表現しません。
