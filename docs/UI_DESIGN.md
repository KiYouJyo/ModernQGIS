# ModernQGIS UI implementation contract

The Figma file below is the visual and interaction source of truth for ModernQGIS shell work:

- File: `boZOYm5aS2SRBI5iu6HfG4`
- Root page: `0:1`
- Reference canvas: `1600 × 980`
- Map Workspace: `5:2`
- Design System: `28:1924`
- Interaction / Accessibility: `30:2043`
- URL: https://www.figma.com/design/boZOYm5aS2SRBI5iu6HfG4/QGIS?node-id=0-1

Do not redraw the shell from memory. Read the relevant Figma node before implementing or materially changing a workspace.

## Core tokens

| Token | Value |
| --- | --- |
| Accent | `#2E875C` |
| Accent soft | `#E3F3EA` |
| Surface | `#FFFFFF` |
| Canvas | `#F5F7F7` |
| Stroke | `#D8DEDE` |
| Text | `#202525` |
| Subtle | `#667070` |
| Dark surface | `#293033` |

Spacing scale: `4 / 8 / 12 / 16 / 24 / 32 px`.

Geometry: radius `4 / 6 / 8 px`; pane border `1 px`; title bar `40 px`; ribbon tabs `36 px`; ribbon workspace `104 px`; status bar `28 px`.

Typography implementation target is **Segoe UI Variable / Windows system UI**. Inter is the design-side fallback in Figma, not an application dependency.

## Reference shell geometry

- Minimum recommended window: `1180 × 720`.
- Default Contents pane: `286 px` in Map Workspace; general allowed default range `286–320 px`.
- Default Layer Properties pane: `304 px`; general allowed default range `304–420 px`.
- The map/data canvas wins horizontal space before persistent panes.
- At narrow widths the right properties pane collapses before the map canvas.
- Ribbon groups overflow horizontally before wrapping.

## Component mapping

| Figma concept | Qt implementation |
| --- | --- |
| custom title bar | `ShellTitleBar` inside `ModernShellWindow` |
| ribbon tabs/workspace | reusable shell widgets built by `ModernShellWindow` |
| document tabs | `QTabWidget` / document-mode tab bar |
| Contents | resizable pane backed by `PaneRegistry` |
| Layer Properties | resizable pane backed by `PaneRegistry` |
| command item | `QAction` registered in `CommandRegistry` |
| map workspace context | `ContextRegistry` key `workspace.map` |
| QGIS native feature | `QgisBridge` public `QgisInterface` binding |

The presentation layer must never call private QGIS object names as a stable feature contract. Public `QgisInterface` actions are preferred. Generic QAction/QDockWidget discovery remains diagnostic only.

## Interaction contract

- `Ctrl+K`: global command search.
- `Ctrl+O`: open project.
- `Ctrl+S`: save project.
- `Ctrl+Shift+S`: save project as.
- `F2`: rename selected layer/item.
- `Space`: toggle selected layer visibility.
- `Ctrl+J`: zoom to selection.
- `Ctrl+T`: attribute table.
- `Ctrl+Enter`: run current processing tool.
- `Esc`: cancel active tool or close transient UI.

Ribbon tabs remain visible when the ribbon workspace is collapsed. Document tabs preserve independent state. Pane placement/width, ribbon collapse and last tool category are user preferences; layer and style state remains project data.

## Accessibility and DPI

- Minimum target `32 × 32 px`; frequent map tools `>= 36 px`.
- Do not communicate visibility, selection or errors by color alone.
- Every icon-only command requires an accessible name and tooltip.
- Respect Windows text scaling, High Contrast and Reduce Motion preferences.
- Keyboard focus order follows shell → pane → canvas → properties.
- Essential text/GIS state targets WCAG AA contrast where applicable.
- 100–200% DPI must not clip command labels.

## Screen inventory

The current Figma page contains 30 reference frames covering the primary shell, Home, Attribute Table, Processing Toolbox, Layout Designer, Settings & Plugins, Styling, Labeling, Data Browser, Plugin Store, Project & CRS, 3D Map, Atlas, Expression Editor, Field Calculator, Layer Properties, connection/export/georeferencing/topology/model workflows, Design System, feedback/state patterns, Dark Theme, raster/temporal/point-cloud/database workspaces, coverage inventory, and interaction/accessibility rules.

v0.3 establishes the reusable shell and Map Workspace first. Later workspaces should compose the same primitives rather than create parallel visual systems.
