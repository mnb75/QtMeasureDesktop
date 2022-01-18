#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsView;
class QLabel;
class TScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handle_btnClose_click();
    void handle_btnMinimize_click();
    void handle_mode_change(bool toggled);
private:
    Ui::MainWindow *ui;

    TScene *_scene;
    QGraphicsView *_view;

    void initScene(QWidget *parent = 0);
};

#endif // MAINWINDOW_H
