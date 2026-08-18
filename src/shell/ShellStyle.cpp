// SPDX-License-Identifier: GPL-2.0-or-later
#include "shell/ShellStyle.h"

namespace modernqgis::shellstyle {

QString accent() { return QStringLiteral("#2E875C"); }
QString accentSoft() { return QStringLiteral("#E3F3EA"); }
QString surface() { return QStringLiteral("#FFFFFF"); }
QString canvas() { return QStringLiteral("#F5F7F7"); }
QString stroke() { return QStringLiteral("#D8DEDE"); }
QString text() { return QStringLiteral("#202525"); }
QString subtle() { return QStringLiteral("#667070"); }

QString lightStyleSheet() {
    return QStringLiteral(R"QSS(
        QMainWindow, QWidget#ModernShellRoot { background: #F5F7F7; color: #202525; }
        QWidget { color: #202525; font-family: "Segoe UI Variable", "Segoe UI"; font-size: 12px; }
        QWidget#TitleBar, QWidget#RibbonTabs, QWidget#RibbonWorkspace, QWidget#StatusBar,
        QWidget#PaneHeader, QWidget#ViewTabs { background: #FFFFFF; }
        QWidget#TitleBar, QWidget#RibbonTabs, QWidget#RibbonWorkspace, QWidget#StatusBar,
        QWidget#Pane, QWidget#ViewTabs { border-bottom: 1px solid #D8DEDE; }
        QWidget#Pane { background: #FFFFFF; border-right: 1px solid #D8DEDE; }
        QWidget#RightPane { background: #FFFFFF; border-left: 1px solid #D8DEDE; }
        QLabel#AppMark { background: #2E875C; color: white; border-radius: 5px; font-size: 15px; font-weight: 600; }
        QLabel#WindowTitle, QLabel#PaneTitle { font-weight: 600; }
        QLineEdit { background: #FFFFFF; border: 1px solid #D8DEDE; border-radius: 4px; padding: 0 9px; min-height: 28px; }
        QLineEdit:focus { border: 1px solid #2E875C; }
        QPushButton { background: transparent; border: 0; border-radius: 4px; padding: 6px 10px; }
        QPushButton:hover { background: #F0F3F3; }
        QPushButton:pressed { background: #E5EAEA; }
        QPushButton#WindowButton { min-width: 42px; max-width: 42px; min-height: 40px; max-height: 40px; border-radius: 0; }
        QPushButton#CloseButton:hover { background: #C42B1C; color: white; }
        QPushButton#RibbonTab { min-height: 34px; padding: 0 17px; border-radius: 0; }
        QPushButton#RibbonTab[active="true"] { color: #202525; font-weight: 600; border-bottom: 2px solid #2E875C; }
        QFrame#RibbonGroup { background: #FFFFFF; border-right: 1px solid #E7EBEB; }
        QLabel#RibbonGroupTitle { color: #667070; font-size: 11px; }
        QToolButton { background: transparent; border: 0; border-radius: 4px; padding: 5px 7px; min-width: 42px; }
        QToolButton:hover { background: #F0F3F3; }
        QToolButton:checked { background: #E3F3EA; color: #2E875C; }
        QTreeWidget, QListWidget, QTableWidget { background: #FFFFFF; border: 0; outline: 0; }
        QTreeWidget::item, QListWidget::item { min-height: 28px; padding-left: 4px; }
        QTreeWidget::item:selected, QListWidget::item:selected { background: #E3F3EA; color: #202525; }
        QTabBar::tab { background: #FFFFFF; border: 0; border-right: 1px solid #E7EBEB; padding: 10px 16px; min-height: 18px; }
        QTabBar::tab:selected { font-weight: 600; border-bottom: 2px solid #2E875C; }
        QComboBox, QSpinBox, QDoubleSpinBox { background: #FFFFFF; border: 1px solid #D8DEDE; border-radius: 4px; min-height: 28px; padding: 0 8px; }
        QSlider::groove:horizontal { height: 4px; background: #D8DEDE; border-radius: 2px; }
        QSlider::sub-page:horizontal { background: #2E875C; border-radius: 2px; }
        QSlider::handle:horizontal { background: #2E875C; width: 14px; margin: -5px 0; border-radius: 7px; }
        QSplitter::handle { background: #D8DEDE; width: 1px; }
        QScrollBar:vertical { background: transparent; width: 10px; margin: 0; }
        QScrollBar::handle:vertical { background: #C5CCCC; min-height: 28px; border-radius: 5px; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QLabel#Subtle { color: #667070; font-size: 11px; }
        QFrame#LayerCard { background: #F7F9F9; border: 1px solid #D8DEDE; border-radius: 5px; }
        QPushButton#PrimaryButton { background: #2E875C; color: white; border: 1px solid #2E875C; }
        QPushButton#SecondaryButton { background: #FFFFFF; border: 1px solid #D8DEDE; }
    )QSS");
}

} // namespace modernqgis::shellstyle
