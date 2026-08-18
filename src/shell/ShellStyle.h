// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <QString>

namespace modernqgis::shellstyle {

inline constexpr int TitleBarHeight = 40;
inline constexpr int RibbonTabsHeight = 36;
inline constexpr int RibbonHeight = 104;
inline constexpr int StatusBarHeight = 28;
inline constexpr int LeftPaneWidth = 286;
inline constexpr int RightPaneWidth = 304;
inline constexpr int MinimumWindowWidth = 1180;
inline constexpr int MinimumWindowHeight = 720;

inline constexpr int SpaceMicro = 4;
inline constexpr int SpaceControl = 8;
inline constexpr int SpaceCompact = 12;
inline constexpr int SpaceStandard = 16;
inline constexpr int SpaceSection = 24;
inline constexpr int SpacePage = 32;

QString lightStyleSheet();
QString accent();
QString accentSoft();
QString surface();
QString canvas();
QString stroke();
QString text();
QString subtle();

} // namespace modernqgis::shellstyle
