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
#include <QColorDialog>

#include "t_scene.h"

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

void MainWindow::btnCloseClickHandle()
{
    close();
}

void MainWindow::btnMinimizeClickHandle()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::btnColorClickHandle()
{
    QColor newColor = QColorDialog::getColor(_lineColor);
    if (newColor.spec() != QColor::Spec::Invalid) {
        _scene->setLineColor(newColor);
        _lineColor = newColor;
    }
}

void MainWindow::mouseEventModeChangeHandle(bool toggled)
{
    TScene::Mode mode = toggled ? TScene::Mode::DrawMode : TScene::Mode::SelectMode;
    _scene->setMode(mode);
}

void MainWindow::initScene(QWidget *parent)
{
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setGeometry(0, 26, 102, 105);
    groupBox->setFixedSize(105, 135);

    QGridLayout *gridLayout = new QGridLayout(groupBox);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *btnClose = new QPushButton("Close", groupBox);
    QPushButton *btnMinimize = new QPushButton("Minimize", groupBox);
    QPushButton *btnClear = new QPushButton("Clear", groupBox);
    QPushButton *btnColor = new QPushButton("Choose Color", groupBox);
    QRadioButton *rbDraw = new QRadioButton("Draw", groupBox);
    QRadioButton *rbSelect = new QRadioButton("Select", groupBox);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    btnClear->setFixedSize(102, 25);
    btnClose->setFixedSize(102, 25);
    btnMinimize->setFixedSize(102, 25);
    btnColor->setFixedSize(102, 25);

    rbDraw->setFixedSize(45, 15);
    rbDraw->setChecked(true);
    rbSelect->setFixedSize(50, 15);

    gridLayout->addWidget(btnMinimize, 0, 0, 1, 2);
    gridLayout->addWidget(btnClear,    1, 0, 1, 2);
    gridLayout->addWidget(btnColor,    2, 0, 1, 2);
    gridLayout->addWidget(btnClose,    3, 0, 1, 2);
    gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);
    gridLayout->addWidget(rbDraw,      5, 0, 1, 1);
    gridLayout->addWidget(rbSelect,    5, 1, 1, 1);

    groupBox->setLayout(gridLayout);

    _scene = new TScene(groupBox, parent);

    connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(btnCloseClickHandle()));
    connect(btnMinimize, SIGNAL(clicked(bool)), this, SLOT(btnMinimizeClickHandle()));
    connect(btnClear, SIGNAL(clicked(bool)), _scene, SLOT(handle_btnClear_click()));
    connect(btnColor, SIGNAL(clicked(bool)), this, SLOT(btnColorClickHandle()));
    connect(rbDraw, SIGNAL(toggled(bool)), this, SLOT(mouseEventModeChangeHandle(bool)));
}
