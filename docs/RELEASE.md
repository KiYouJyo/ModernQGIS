# ModernQGIS Release Contract

`release/release.json` is the authority for the current target version and publication flags. `CHANGELOG.md` is development history; immutable tags and GitHub Releases are publication facts.

v0.1–v0.2 use GitHub Preview only. A build, artifact, tag, and GitHub Release are separate states.

## Package

```text
ModernQGIS-ShellPreview-vX.Y.Z-windows-x64.zip
SHA256SUMS
```

Foundation packages contain the standalone Qt sandbox/runtime only and **must not bundle QGIS**.

Every publishable version requires `release/notes/vX.Y.Z.zh-CN.md`, `.ja-JP.md`, and `.en-US.md` with equivalent release facts.

## Gate

Before `channels.github.publish=true`: CMake and release versions match; CI/tests/i18n pass; three notes exist; package naming and SHA-256 are correct; no keys/certs/tokens/QGIS binaries/local paths are included; the intended source commit/tag follows maintainer signing policy.

Manual `release-preview.yml` runs only build an artifact. A GitHub Release is created only from a matching `v*` tag with `publish=true`. Published same-version binaries are immutable; fixes require a new version.

CI never stores private signing material. Unsigned API-generated commits must not be presented as Verified.
