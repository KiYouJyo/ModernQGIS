# Upstream Strategy

Use a two-repository model: `ModernQGIS` for the product/research shell; a separate QGIS fork only for minimal upstream API experiments and QGIS PRs. Do not accumulate product code in the QGIS fork.

Likely upstream-friendly: presentation-neutral command metadata, pane lifecycle APIs, workspace/view abstractions, plugin UI registration improvements, and generic capability discovery.

Likely ModernQGIS-specific until proven otherwise: Ribbon visuals, start page, theme/branding, and opinionated information architecture.

When public QGIS APIs are insufficient: document the gap → build the smallest isolated QGIS-fork experiment → confirm generic value → discuss upstream → submit incremental changes, not one giant UI replacement PR.

ModernQGIS may learn from ArcGIS Pro interaction patterns, but creates its own visual identity and does not copy Esri trademarks, proprietary icons, or protected visual assets.
