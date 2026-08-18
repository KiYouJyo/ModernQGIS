// SPDX-License-Identifier: GPL-2.0-or-later
#include "shell/ModernShellWindow.h"

#include "core/CommandDescriptor.h"
#include "core/PaneDescriptor.h"
#include "shell/ShellStyle.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QShortcut>
#include <QSlider>
#include <QSplitter>
#include <QTabBar>
#include <QTabWidget>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWindow>

namespace modernqgis {
namespace {

class ShellTitleBar final : public QWidget {
public:
    explicit ShellTitleBar(QWidget* parent = nullptr) : QWidget(parent) {}
protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && window()->windowHandle()) {
            window()->windowHandle()->startSystemMove();
        }
        QWidget::mousePressEvent(event);
    }
    void mouseDoubleClickEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            window()->isMaximized() ? window()->showNormal() : window()->showMaximized();
        }
        QWidget::mouseDoubleClickEvent(event);
    }
};

class MapCanvasPreview final : public QWidget {
public:
    explicit MapCanvasPreview(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(420, 280);
        setAccessibleName(QObject::tr("Map canvas"));
    }
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), QColor(QStringLiteral("#EEF2ED")));

        const auto sx = width() / 1010.0;
        const auto sy = height() / 734.0;
        auto rr = [sx, sy](double x, double y, double w, double h) {
            return QRectF(x * sx, y * sy, w * sx, h * sy);
        };

        p.setPen(Qt::NoPen);
        p.setBrush(QColor(QStringLiteral("#DDE6D9")));
        const QRectF blocks[] = {
            rr(70,60,160,100), rr(270,55,190,120), rr(500,70,150,90), rr(690,50,210,130),
            rr(100,235,210,140), rr(360,235,160,130), rr(560,230,250,150), rr(825,230,150,120),
            rr(70,430,180,120), rr(290,430,200,130), rr(535,430,180,110), rr(760,430,220,140)
        };
        for (const auto& block : blocks) p.drawRoundedRect(block, 4, 4);

        p.setBrush(QColor(QStringLiteral("#78B9D0")));
        p.save();
        p.translate(width() * .88, -height() * .05);
        p.rotate(12);
        p.drawRoundedRect(QRectF(0, 0, 42 * sx, 780 * sy), 18, 18);
        p.restore();

        p.setBrush(QColor(QStringLiteral("#F8FAF8")));
        p.save();
        p.translate(width() * .01, height() * .25);
        p.rotate(4);
        p.drawRoundedRect(QRectF(0, 0, width() * 1.05, 58 * sy), 3, 3);
        p.restore();
        p.save();
        p.translate(width() * .50, height() * .02);
        p.rotate(-2);
        p.drawRoundedRect(QRectF(0, 0, 48 * sx, height() * .88), 3, 3);
        p.restore();

        QPen boundary(QColor(QStringLiteral("#2E875C")), 2, Qt::DashLine);
        p.setBrush(Qt::NoBrush);
        p.setPen(boundary);
        p.drawRoundedRect(rr(150, 105, 760, 430), 5, 5);
    }
};

QLabel* label(const QString& text, QWidget* parent = nullptr, const char* objectName = nullptr) {
    auto* value = new QLabel(text, parent);
    if (objectName) value->setObjectName(QString::fromLatin1(objectName));
    return value;
}

} // namespace

ModernShellWindow::ModernShellWindow(QWidget* parent)
    : QMainWindow(parent), m_commands(this), m_panes(this), m_contexts(this) {
    setWindowTitle(tr("ModernQGIS — Map Workspace"));
    setMinimumSize(shellstyle::MinimumWindowWidth, shellstyle::MinimumWindowHeight);
    resize(1600, 980);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setStyleSheet(shellstyle::lightStyleSheet());

    QFont appFont(QStringLiteral("Segoe UI Variable"));
    appFont.setPixelSize(12);
    setFont(appFont);

    auto* root = new QWidget(this);
    root->setObjectName(QStringLiteral("ModernShellRoot"));
    auto* rootLayout = new QVBoxLayout(root);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->addWidget(buildTitleBar());
    rootLayout->addWidget(buildRibbonTabs());
    m_ribbonWorkspace = buildRibbonWorkspace();
    rootLayout->addWidget(m_ribbonWorkspace);

    m_workspaceSplitter = new QSplitter(Qt::Horizontal, root);
    m_workspaceSplitter->setChildrenCollapsible(false);
    auto* contents = buildContentsPane();
    auto* map = buildMapWorkspace();
    auto* properties = buildPropertiesPane();
    m_workspaceSplitter->addWidget(contents);
    m_workspaceSplitter->addWidget(map);
    m_workspaceSplitter->addWidget(properties);
    m_workspaceSplitter->setStretchFactor(0, 0);
    m_workspaceSplitter->setStretchFactor(1, 1);
    m_workspaceSplitter->setStretchFactor(2, 0);
    m_workspaceSplitter->setSizes({shellstyle::LeftPaneWidth, 1010, shellstyle::RightPaneWidth});
    rootLayout->addWidget(m_workspaceSplitter, 1);
    rootLayout->addWidget(buildStatusBar());
    setCentralWidget(root);

    PaneDescriptor contentsPane;
    contentsPane.id = QStringLiteral("contents");
    contentsPane.title = tr("Contents");
    contentsPane.preferredLocation = PaneLocation::Left;
    contentsPane.widget = contents;
    m_panes.registerPane(contentsPane);

    PaneDescriptor propertiesPane;
    propertiesPane.id = QStringLiteral("properties");
    propertiesPane.title = tr("Layer Properties");
    propertiesPane.preferredLocation = PaneLocation::Right;
    propertiesPane.widget = properties;
    m_panes.registerPane(propertiesPane);
    m_contexts.activate(QStringLiteral("workspace.map"));

    auto* searchShortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+K")), this);
    connect(searchShortcut, &QShortcut::activated, this, [this] {
        m_globalSearch->setFocus(Qt::ShortcutFocusReason);
        m_globalSearch->selectAll();
    });
    restoreShellState();
}

QWidget* ModernShellWindow::buildTitleBar() {
    auto* bar = new ShellTitleBar(this);
    bar->setObjectName(QStringLiteral("TitleBar"));
    bar->setFixedHeight(shellstyle::TitleBarHeight);
    auto* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(12, 0, 8, 0);
    layout->setSpacing(10);

    auto* mark = label(QStringLiteral("Q"), bar, "AppMark");
    mark->setAlignment(Qt::AlignCenter);
    mark->setFixedSize(24, 24);
    layout->addWidget(mark);
    layout->addWidget(label(tr("ModernQGIS · City Planning.qgz"), bar, "WindowTitle"));
    layout->addStretch(1);

    m_globalSearch = new QLineEdit(bar);
    m_globalSearch->setPlaceholderText(tr("Search tools, commands, layers, and settings  Ctrl+K"));
    m_globalSearch->setAccessibleName(tr("Global command search"));
    m_globalSearch->setFixedSize(320, 30);
    layout->addWidget(m_globalSearch);

    const auto windowButton = [bar](const QString& text, const QString& accessible) {
        auto* button = new QPushButton(text, bar);
        button->setObjectName(QStringLiteral("WindowButton"));
        button->setAccessibleName(accessible);
        button->setToolTip(accessible);
        return button;
    };
    auto* minimize = windowButton(QStringLiteral("—"), tr("Minimize"));
    auto* maximize = windowButton(QStringLiteral("□"), tr("Maximize or restore"));
    auto* close = windowButton(QStringLiteral("×"), tr("Close"));
    close->setObjectName(QStringLiteral("CloseButton"));
    layout->addWidget(minimize);
    layout->addWidget(maximize);
    layout->addWidget(close);
    connect(minimize, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(maximize, &QPushButton::clicked, this, [this] { isMaximized() ? showNormal() : showMaximized(); });
    connect(close, &QPushButton::clicked, this, &QWidget::close);
    return bar;
}

QWidget* ModernShellWindow::buildRibbonTabs() {
    auto* tabs = new QWidget(this);
    tabs->setObjectName(QStringLiteral("RibbonTabs"));
    tabs->setFixedHeight(shellstyle::RibbonTabsHeight);
    auto* layout = new QHBoxLayout(tabs);
    layout->setContentsMargins(12, 0, 8, 0);
    layout->setSpacing(2);
    const QStringList names = {tr("Project"), tr("Map"), tr("Insert"), tr("Select"), tr("Analysis"), tr("Edit"), tr("View"), tr("Help")};
    for (int i = 0; i < names.size(); ++i) {
        auto* button = new QPushButton(names.at(i), tabs);
        button->setObjectName(QStringLiteral("RibbonTab"));
        button->setProperty("active", i == 1);
        button->setAccessibleName(names.at(i));
        layout->addWidget(button);
    }
    layout->addStretch(1);
    auto* collapse = new QPushButton(QStringLiteral("⌃"), tabs);
    collapse->setAccessibleName(tr("Collapse ribbon"));
    collapse->setToolTip(tr("Collapse ribbon"));
    layout->addWidget(collapse);
    connect(collapse, &QPushButton::clicked, this, [this, collapse] {
        setRibbonCollapsed(!m_ribbonCollapsed);
        collapse->setText(m_ribbonCollapsed ? QStringLiteral("⌄") : QStringLiteral("⌃"));
    });
    return tabs;
}

QWidget* ModernShellWindow::makeRibbonGroup(const QString& title, const QList<QPair<QString, QString>>& commands) {
    auto* group = new QFrame(this);
    group->setObjectName(QStringLiteral("RibbonGroup"));
    auto* layout = new QVBoxLayout(group);
    layout->setContentsMargins(8, 7, 8, 4);
    layout->setSpacing(3);
    auto* commandRow = new QHBoxLayout();
    commandRow->setSpacing(2);
    for (const auto& item : commands) {
        auto* action = new QAction(item.second, group);
        auto* button = new QToolButton(group);
        button->setDefaultAction(action);
        button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        button->setMinimumSize(54, 62);
        button->setAccessibleName(item.second);
        button->setToolTip(item.second);
        commandRow->addWidget(button);
        registerShellCommand(item.first, item.second, action, title.toLower());
    }
    layout->addLayout(commandRow, 1);
    auto* groupTitle = label(title, group, "RibbonGroupTitle");
    groupTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(groupTitle);
    return group;
}

QWidget* ModernShellWindow::buildRibbonWorkspace() {
    auto* ribbon = new QWidget(this);
    ribbon->setObjectName(QStringLiteral("RibbonWorkspace"));
    ribbon->setFixedHeight(shellstyle::RibbonHeight);
    auto* layout = new QHBoxLayout(ribbon);
    layout->setContentsMargins(4, 0, 4, 0);
    layout->setSpacing(0);
    layout->addWidget(makeRibbonGroup(tr("Navigate"), {
        {QStringLiteral("map.explore"), tr("Explore")},
        {QStringLiteral("map.pan"), tr("Pan")},
        {QStringLiteral("map.full-extent"), tr("Full Extent")}
    }));
    layout->addWidget(makeRibbonGroup(tr("Layer"), {
        {QStringLiteral("layer.add-data"), tr("Add Data")},
        {QStringLiteral("layer.properties"), tr("Properties")},
        {QStringLiteral("layer.attribute-table"), tr("Attribute Table")}
    }));
    layout->addWidget(makeRibbonGroup(tr("Selection"), {
        {QStringLiteral("map.select"), tr("Select")},
        {QStringLiteral("map.zoom-selection"), tr("Zoom To")},
        {QStringLiteral("map.clear-selection"), tr("Clear")}
    }));
    layout->addWidget(makeRibbonGroup(tr("Measure"), {
        {QStringLiteral("map.measure"), tr("Distance")},
        {QStringLiteral("map.measure-area"), tr("Area")}
    }));
    layout->addStretch(1);
    return ribbon;
}

QWidget* ModernShellWindow::buildContentsPane() {
    auto* pane = new QWidget(this);
    pane->setObjectName(QStringLiteral("Pane"));
    pane->setMinimumWidth(220);
    pane->setMaximumWidth(520);
    auto* layout = new QVBoxLayout(pane);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* header = new QWidget(pane);
    header->setObjectName(QStringLiteral("PaneHeader"));
    header->setFixedHeight(42);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 0, 10, 0);
    headerLayout->addWidget(label(tr("Contents"), header, "PaneTitle"));
    headerLayout->addStretch(1);
    headerLayout->addWidget(label(QStringLiteral("⋯"), header));
    headerLayout->addWidget(label(QStringLiteral("×"), header));
    layout->addWidget(header);

    auto* searchWrap = new QWidget(pane);
    searchWrap->setFixedHeight(40);
    auto* searchLayout = new QHBoxLayout(searchWrap);
    searchLayout->setContentsMargins(10, 6, 10, 6);
    auto* search = new QLineEdit(searchWrap);
    search->setPlaceholderText(tr("Search layers"));
    search->setAccessibleName(tr("Search layers"));
    searchLayout->addWidget(search);
    layout->addWidget(searchWrap);

    auto* modes = new QTabBar(pane);
    modes->setExpanding(true);
    modes->setFixedHeight(34);
    modes->addTab(tr("Drawing Order"));
    modes->addTab(tr("Data Source"));
    modes->addTab(tr("Selection"));
    layout->addWidget(modes);

    m_layerTree = new QTreeWidget(pane);
    m_layerTree->setHeaderHidden(true);
    m_layerTree->setRootIsDecorated(true);
    auto* project = new QTreeWidgetItem(QStringList{tr("City Planning Project")});
    m_layerTree->addTopLevelItem(project);
    const QStringList layers = {tr("Planning Boundary"), tr("Road Centerlines"), tr("Rivers"), tr("Building Footprints"), tr("Existing Green Space"), tr("Satellite Imagery")};
    for (const auto& name : layers) {
        auto* item = new QTreeWidgetItem(project, QStringList{name});
        item->setCheckState(0, Qt::Checked);
    }
    project->setExpanded(true);
    layout->addWidget(m_layerTree, 1);
    return pane;
}

QWidget* ModernShellWindow::buildMapWorkspace() {
    auto* wrapper = new QWidget(this);
    wrapper->setObjectName(QStringLiteral("MapWorkspace"));
    auto* layout = new QVBoxLayout(wrapper);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_documentTabs = new QTabWidget(wrapper);
    m_documentTabs->setTabsClosable(true);
    m_documentTabs->setMovable(true);
    m_documentTabs->setDocumentMode(true);
    auto* map = new MapCanvasPreview(m_documentTabs);
    m_documentTabs->addTab(map, tr("Map"));
    auto* layoutPlaceholder = new QLabel(tr("Layout workspace"), m_documentTabs);
    layoutPlaceholder->setAlignment(Qt::AlignCenter);
    m_documentTabs->addTab(layoutPlaceholder, tr("Layout 1"));
    layout->addWidget(m_documentTabs, 1);
    return wrapper;
}

QWidget* ModernShellWindow::buildPropertiesPane() {
    auto* pane = new QWidget(this);
    pane->setObjectName(QStringLiteral("RightPane"));
    pane->setMinimumWidth(240);
    pane->setMaximumWidth(560);
    auto* layout = new QVBoxLayout(pane);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* header = new QWidget(pane);
    header->setObjectName(QStringLiteral("PaneHeader"));
    header->setFixedHeight(42);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 0, 10, 0);
    headerLayout->addWidget(label(tr("Layer Properties"), header, "PaneTitle"));
    headerLayout->addStretch(1);
    headerLayout->addWidget(label(QStringLiteral("⋯"), header));
    headerLayout->addWidget(label(QStringLiteral("×"), header));
    layout->addWidget(header);

    auto* tabs = new QTabBar(pane);
    tabs->setExpanding(true);
    tabs->setFixedHeight(36);
    tabs->addTab(tr("General"));
    tabs->addTab(tr("Symbology"));
    tabs->addTab(tr("Labels"));
    layout->addWidget(tabs);

    auto* content = new QWidget(pane);
    auto* form = new QVBoxLayout(content);
    form->setContentsMargins(14, 14, 14, 14);
    form->setSpacing(12);

    auto* card = new QFrame(content);
    card->setObjectName(QStringLiteral("LayerCard"));
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(12, 10, 12, 10);
    cardLayout->setSpacing(4);
    cardLayout->addWidget(label(tr("Road Centerlines"), card, "PaneTitle"));
    cardLayout->addWidget(label(tr("LineString · 1,284 features"), card, "Subtle"));
    form->addWidget(card);

    const auto addComboRow = [content, form](const QString& name, const QStringList& values) {
        auto* row = new QWidget(content);
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        auto* field = new QLabel(name, row);
        field->setMinimumWidth(92);
        auto* combo = new QComboBox(row);
        combo->addItems(values);
        combo->setAccessibleName(name);
        rowLayout->addWidget(field);
        rowLayout->addWidget(combo, 1);
        form->addWidget(row);
    };
    addComboRow(tr("Renderer"), {tr("Single Symbol")});
    addComboRow(tr("Blend Mode"), {tr("Normal")});
    addComboRow(tr("Visible Scale"), {tr("All Scales")});

    form->addWidget(label(tr("Opacity"), content));
    auto* slider = new QSlider(Qt::Horizontal, content);
    slider->setRange(0, 100);
    slider->setValue(86);
    slider->setAccessibleName(tr("Opacity"));
    form->addWidget(slider);

    auto* actions = new QHBoxLayout();
    auto* apply = new QPushButton(tr("Apply"), content);
    apply->setObjectName(QStringLiteral("PrimaryButton"));
    auto* reset = new QPushButton(tr("Reset"), content);
    reset->setObjectName(QStringLiteral("SecondaryButton"));
    actions->addWidget(apply);
    actions->addWidget(reset);
    actions->addStretch(1);
    form->addLayout(actions);
    form->addStretch(1);
    layout->addWidget(content, 1);
    return pane;
}

QWidget* ModernShellWindow::buildStatusBar() {
    auto* bar = new QWidget(this);
    bar->setObjectName(QStringLiteral("StatusBar"));
    bar->setFixedHeight(shellstyle::StatusBarHeight);
    auto* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(12, 0, 12, 0);
    layout->setSpacing(18);
    layout->addWidget(label(QStringLiteral("120.1532°E, 30.2741°N"), bar, "Subtle"));
    layout->addWidget(label(tr("Scale 1:10,000"), bar, "Subtle"));
    layout->addWidget(label(tr("Rotation 0°"), bar, "Subtle"));
    layout->addWidget(label(QStringLiteral("EPSG:4490"), bar, "Subtle"));
    layout->addStretch(1);
    layout->addWidget(label(tr("✓ Rendering complete"), bar, "Subtle"));
    return bar;
}

void ModernShellWindow::registerShellCommand(const QString& id, const QString& title, QAction* action, const QString& category) {
    CommandDescriptor descriptor;
    descriptor.id = id;
    descriptor.title = title;
    descriptor.category = category;
    descriptor.action = action;
    m_commands.registerCommand(descriptor);
}

void ModernShellWindow::setRibbonCollapsed(bool collapsed) {
    m_ribbonCollapsed = collapsed;
    m_ribbonWorkspace->setVisible(!collapsed);
}

void ModernShellWindow::restoreShellState() {
    QSettings settings;
    setRibbonCollapsed(settings.value(QStringLiteral("shell/ribbonCollapsed"), false).toBool());
    const auto sizes = settings.value(QStringLiteral("shell/paneSizes")).value<QList<int>>();
    if (sizes.size() == 3) m_workspaceSplitter->setSizes(sizes);
}

void ModernShellWindow::saveShellState() const {
    QSettings settings;
    settings.setValue(QStringLiteral("shell/ribbonCollapsed"), m_ribbonCollapsed);
    settings.setValue(QStringLiteral("shell/paneSizes"), QVariant::fromValue(m_workspaceSplitter->sizes()));
}

void ModernShellWindow::closeEvent(QCloseEvent* event) {
    saveShellState();
    QMainWindow::closeEvent(event);
}

} // namespace modernqgis
