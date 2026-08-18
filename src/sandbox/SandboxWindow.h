// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "shell/ModernShellWindow.h"

namespace modernqgis {

class SandboxWindow final : public ModernShellWindow {
    Q_OBJECT
public:
    explicit SandboxWindow(QWidget* parent = nullptr);
};

} // namespace modernqgis
