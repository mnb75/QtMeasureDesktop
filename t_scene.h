#ifndef T_SCENE_H
#define T_SCENE_H

#include <QGraphicsScene>

class QLabel;
class TLineLabelItem;
class QGroupBox;

class TScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {
        DrawMode = 0,
        SelectMode
    };

    explicit TScene(QWidget *widget, QObject *parent = nullptr);

    void setMode(Mode mode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);

public slots:
    void handle_btnClear_click();

private:
    Mode _mode;

    // line points(start and end)
    QPointF _startPoint;
    QPointF _endPoint;

    bool _isLeftClickPressed = false;
    bool _isShiftKeyPressed = false;

    TLineLabelItem *_lineLabelItem = nullptr;

    // check for using better data structure
    QList<QLabel*> _labels;
};

#endif // T_SCENE_H
