#ifndef T_SCENE_H
#define T_SCENE_H

#include <QGraphicsScene>

class TLineLabelItem;

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
    void setLineColor(QColor color);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);

public slots:
    void handle_btnClear_click();

private:
    Mode _mode;

    QColor _lineColor = Qt::black;

    // line points(start and end)
    QPointF _startPoint;
    QPointF _endPoint;

    bool _isLeftClickPressed = false;
    bool _isShiftKeyPressed  = false;
    bool _isWidgetPressed    = false;

    TLineLabelItem *_lineLabelItem = nullptr;
};

#endif // T_SCENE_H
