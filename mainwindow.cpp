#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRect>
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>

#include <QLabel>
#include <QLine>
#include <QtMath>
#include <QPushButton>

#include <QDebug>

// TODOS:
// (done)1-push buttons for closing, minimizing and clearing
//       2-selecting and translating and deleteng each single drawed line
//       3-handle right click for cancling just drawing line
//       4-add setting button for some customization like change line color and etc.
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

    // if mouse tracking is enabled, the widget receives mouse move events even if no buttons are pressed
    centralWidget()->setAttribute(Qt::WA_TransparentForMouseEvents);
    setMouseTracking(true);

    ui->groupBox->setStyleSheet("QGroupBox {background-color: rgb(255, 255, 255);}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *mouseEvent)
{
    QRect gbxGeometry = ui->groupBox->geometry();

    if (mouseEvent->pos().x() >= gbxGeometry.x()
            && mouseEvent->pos().x() <= gbxGeometry.x() + gbxGeometry.width()
            && mouseEvent->pos().y() >= gbxGeometry.y()
            && mouseEvent->pos().y() <= gbxGeometry.y() + gbxGeometry.height()) { // click inside groupbox

        QRect btnMinimizeGeometry = ui->btnMinimize->geometry();
        QRect btnCloseGeometry    = ui->btnClose->geometry();
        QRect btnClearGeometry    = ui->btnClear->geometry();

        if (mouseEvent->pos().x() >= btnMinimizeGeometry.x()
                && mouseEvent->pos().x() <= btnMinimizeGeometry.x() + btnMinimizeGeometry.width()
                && mouseEvent->pos().y() >= btnMinimizeGeometry.y()
                && mouseEvent->pos().y() <= btnMinimizeGeometry.y() + btnMinimizeGeometry.height()) { // click btnMinimize
            ui->btnMinimize->click();
        } else if (mouseEvent->pos().x() >= btnCloseGeometry.x()
                   && mouseEvent->pos().x() <= btnCloseGeometry.x() + btnCloseGeometry.width()
                   && mouseEvent->pos().y() >= btnCloseGeometry.y()
                   && mouseEvent->pos().y() <= btnCloseGeometry.y() + btnCloseGeometry.height()) { // click btnCloseGeometry
            ui->btnClose->click();
        } else if (mouseEvent->pos().x() >= btnClearGeometry.x()
                   && mouseEvent->pos().x() <= btnClearGeometry.x() + btnClearGeometry.width()
                   && mouseEvent->pos().y() >= btnClearGeometry.y()
                   && mouseEvent->pos().y() <= btnClearGeometry.y() + btnClearGeometry.height()) { // click btnClearGeometry
            ui->btnClear->click();
        }

        QMainWindow::mousePressEvent(mouseEvent);

    } else {
        if ((mouseEvent->button() == Qt::LeftButton)) {
            if (!_isLeftClickPressed) {
                _isLeftClickPressed = true;
                _startPoint = mouseEvent->pos();

                QLabel *label = new QLabel(centralWidget());
                label->setGeometry(_startPoint.x(), _startPoint.y(), 50, 50);
                label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                label->setStyleSheet("QLabel {color:blue;}");
                label->setWindowFlag(Qt::FramelessWindowHint); // No frame
                label->setAttribute(Qt::WA_NoSystemBackground); // No background
                label->setAttribute(Qt::WA_TranslucentBackground);
                QFont font = label->font();
                font.setPixelSize(13);
                label->setFont(font);
                label->show();

                _labels.append(label);

                QLineF line;
                line.setP1(_startPoint);
                _lines.append(line);

            } else {
                _isLeftClickPressed = false;
                _endPoint = mouseEvent->pos();
                if (_isShiftKeyPressed) {
                    QLineF line;
                    line.setP1(_startPoint);
                    line.setP2(mouseEvent->pos());
                    double angle = line.angle(); // in degree(between 0 to 360 ccw)

                    QPointF point;
                    if (angle <= 45.0 || angle >= 315.0 || (angle >= 135.0 && angle <= 225.0)) { // projected on x axis
                        point.setX(mouseEvent->pos().x());
                        point.setY(_startPoint.y());

                    } else { // projected on y axis
                        point.setX(_startPoint.x());
                        point.setY(mouseEvent->pos().y());

                    }
                    _lines.last().setP2(point);
                } else {
                    _lines.last().setP2(_endPoint);
                }
                update();
            }
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    if (_isLeftClickPressed) {
        if (_isShiftKeyPressed) {
            QLineF line;
            line.setP1(_startPoint);
            line.setP2(mouseEvent->pos());
            double angle = line.angle(); // in degree(between 0 to 360 ccw)

            QPointF point;
            if (angle <= 35.0 || angle >= 325.0 || (angle >= 145.0 && angle <= 215.0)) { // projected on x axis
                point.setX(mouseEvent->pos().x());
                point.setY(_startPoint.y());

            } else if ((angle > 35.0 && angle < 55.0) || (angle > 125.0 && angle < 145.0)
                       || (angle > 215.0 && angle < 235.0) || (angle > 305.0 && angle < 325.0)) { // projected on 45deg
                point.setX(mouseEvent->pos().x());
                double dx = line.dx();
                angle = (angle > 35.0 && angle < 55.0) ? 45.0 : (angle > 125.0 && angle < 145.0)
                                                         ? 135.0 : (angle > 215.0 && angle < 235.0) ? 225.0 : 315.0;
                double vatar = dx / qSin(angle * M_PI / 180.0);
                int sign = (angle > 35.0 && angle < 55.0) ? -1 : (angle > 125.0 && angle < 145.0) ? -1 : 1;
                double dy = sign * qSqrt(vatar * vatar - dx * dx);
                point.setY(dy + line.y1());

            } else { // projected on y axis
                point.setX(_startPoint.x());
                point.setY(mouseEvent->pos().y());

            }
            _lines.last().setP2(point);

        } else {
            _lines.last().setP2(mouseEvent->pos());
        }
        _labels.last()->setText(QString::number(_lines.last().length()));
        _labels.last()->setGeometry(_lines.last().center().x(), _lines.last().center().y(), 50, 50);
        update();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Shift)
        _isShiftKeyPressed = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Shift)
        _isShiftKeyPressed = false;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (_lines.count() != 0 && _labels.count() != 0) {
        QPainter painter(this);
        painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
        painter.drawLines(_lines);
    }
}


void MainWindow::on_btnClose_clicked()
{
    close();
}

void MainWindow::on_btnMinimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

void MainWindow::on_btnClear_clicked()
{
    if (_labels.count() && _lines.count()) {
        qDeleteAll(_labels);
        _lines.clear();
        _labels.clear();
        update();
    }
}
