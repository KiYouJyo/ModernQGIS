# Localization Contract

Foundation requires three maintained locales: `zh-CN`/`zh_CN` (简体中文), `ja-JP`/`ja_JP` (日本語), and `en-US`/`en_US` (English).

User-visible Qt strings use `tr()`/Qt translation mechanisms. Do not build sentences from fragments translators cannot reorder. Translation sources live in `translations/modernqgis_*.ts`.

`scripts/check_i18n.py` requires identical `(context, source)` key sets and non-empty translations across all three catalogs.

Root README and publishable release notes are maintained in all three languages. Engineering-only design records may remain English-first when translation would make synchronization unreliable.

`LocalizationManager` normalizes Chinese to zh_CN, Japanese to ja_JP, and other locales to en_US, then loads the embedded `.qm` resource.
