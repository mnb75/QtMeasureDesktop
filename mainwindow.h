#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsView;
class TScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void btnCloseClickHandle();
    void btnMinimizeClickHandle();
    void btnColorClickHandle();
    void mouseEventModeChangeHandle(bool toggled);

private:
    Ui::MainWindow *ui;

    TScene *_scene;
    QGraphicsView *_view;

    QColor _lineColor = Qt::black;

    void initScene(QWidget *parent = 0);
};

#endif // MAINWINDOW_H
