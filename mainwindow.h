#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void paintEvent(QPaintEvent *event);

private slots:
    void on_btnClose_clicked();
    void on_btnMinimize_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;

    QPointF _startPoint;
    QPointF _endPoint;

    bool _isLeftClickPressed = false;
    bool _isShiftKeyPressed = false;

    // check for using better data structure
    QVector<QLineF> _lines;
    QList<QLabel*> _labels;
};

#endif // MAINWINDOW_H
