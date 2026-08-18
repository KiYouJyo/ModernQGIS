# Repository Contract

ModernQGIS is a standalone repository. QGIS source code, QGIS build trees, user profiles, signing material, and locally installed QGIS binaries are never vendored into this repository.

## Layout

```text
ModernQGIS/
├─ .github/                 # CI, packaging, PR and issue contracts
├─ cmake/                   # External dependency discovery only
├─ docs/                    # Architecture and engineering contracts
├─ release/                 # Release target SSOT and multilingual notes
├─ scripts/                 # Repository-level validation
├─ src/
│  ├─ core/                 # Presentation-neutral registries/contracts
│  ├─ bridge/               # QGIS boundary and standalone null bridge
│  ├─ i18n/                 # Runtime locale selection
│  ├─ plugin/               # Optional QGIS host entry point
│  └─ sandbox/              # Standalone foundation executable
├─ tests/                   # Foundation unit/contract tests
└─ translations/            # zh_CN / ja_JP / en_US Qt catalogs
```

## Dependency direction

```text
sandbox / future shell
          ↓
core + i18n
          ↑
       bridge
          ↑
    QGIS public API
```

`src/core/` must not include QGIS headers. QGIS-specific includes belong in `src/bridge/`, `src/plugin/`, or a future `src/compat/`. Visual shell code must not call QGIS private implementation details directly.

## Naming

- Product: `ModernQGIS`
- Stable command IDs: lowercase dotted IDs (`project.open`, `map.pan`)
- Branches: `agent/<goal>`, `feature/<goal>`, `fix/<goal>`
- Version tags: `vMAJOR.MINOR.PATCH`
- Preview package: `ModernQGIS-ShellPreview-vX.Y.Z-<platform>-<arch>.zip`

## Generated and external content

Build trees, `.qm` outputs, archives, signing files, local environment files, QGIS binaries, and QGIS profiles are ignored or rejected by CI. Third-party dependencies remain external unless a future reviewed dependency policy explicitly allows vendoring.
