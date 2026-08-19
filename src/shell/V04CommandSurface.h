// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

namespace modernqgis {

class ModernShellWindow;

// Adds v0.4 functional commands to the existing Figma-led ribbon without
// introducing QGIS API types into the presentation layer.
void installV04CommandSurface(ModernShellWindow* shell);

} // namespace modernqgis
