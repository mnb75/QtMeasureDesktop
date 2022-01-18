#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QSpacerItem>

#include "t_central_widget.h"
#include "t_scene.h"

// TODOS:
// (done)1-push buttons for closing, minimizing and clearing
//       2-selecting and translating and deleting each single drawed line
//       3-handle right click for cancling just drawing line
//       4-add setting button for some customization like change line color and etc.
//       5-use qgraphicview
//       4-refactoring
//       5-transfer to git version control
//       6-push to github

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // for generating a transparent window
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background-color: rgba(128, 128, 128, 100);");

    // enlarge font size
    QFont custom_font;
    custom_font.setPixelSize(25);
    setFont(custom_font);

    // setting size of window accroding to available geometry
    // available geometry: total geometry minus taskbar geometry
    const QRect rec = QApplication::desktop()->availableGeometry();
    int height = rec.height();
    int width = rec.width();
    setFixedSize(width, height);

    initScene(parent);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    _view = new QGraphicsView(_scene);
    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _view->setFixedSize(width, height);
    _view->setSceneRect(0, 0, width, height);

    layout->addWidget(_view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("Measure Desktop"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_btnClose_click()
{
    close();
}

void MainWindow::handle_btnMinimize_click()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::handle_mode_change(bool toggled)
{
    TScene::Mode mode = toggled ? TScene::Mode::DrawMode : TScene::Mode::SelectMode;
    _scene->setMode(mode);
}

void MainWindow::initScene(QWidget *parent)
{
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setGeometry(0, 26, 102, 105);
    groupBox->setFixedSize(105, 105);

    QGridLayout *gridLayout = new QGridLayout(groupBox);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *btnClose = new QPushButton("Close", groupBox);
    QPushButton *btnMinimize = new QPushButton("Minimize", groupBox);
    QPushButton *btnClear = new QPushButton("Clear", groupBox);
    QRadioButton *rbDraw = new QRadioButton("Draw", groupBox);
    QRadioButton *rbSelect = new QRadioButton("Select", groupBox);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    btnClear->setFixedSize(102, 25);
    btnClose->setFixedSize(102, 25);
    btnMinimize->setFixedSize(102, 25);

    rbDraw->setFixedSize(45, 15);
    rbDraw->setChecked(true);
    rbSelect->setFixedSize(50, 15);

    gridLayout->addWidget(btnMinimize, 0, 0, 1, 2);
    gridLayout->addWidget(btnClear,    1, 0, 1, 2);
    gridLayout->addWidget(btnClose,    2, 0, 1, 2);
    gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);
    gridLayout->addWidget(rbDraw,      4, 0, 1, 1);
    gridLayout->addWidget(rbSelect,    4, 1, 1, 1);

    groupBox->setLayout(gridLayout);

    _scene = new TScene(groupBox, parent);

    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(handle_btnClose_click()));
    connect(btnMinimize, SIGNAL(clicked(bool)), this, SLOT(handle_btnMinimize_click()));
    connect(btnClear, SIGNAL(clicked(bool)), _scene, SLOT(handle_btnClear_click()));
    connect(rbDraw, SIGNAL(toggled(bool)), this, SLOT(handle_mode_change(bool)));
}
