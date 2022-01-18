#include "t_scene.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QLabel>

#include <QtMath>
#include <QKeyEvent>

#include "t_line.h"
#include "t_groupbox_item.h"

TScene::TScene(QWidget *widget, QObject *parent)
    : QGraphicsScene(parent), _mode(DrawMode)
{
    // add movable widget to the top left of the window
    auto *item = new TMovableWidgetItem(widget);
    addItem(item);
}

void TScene::setMode(TScene::Mode mode)
{
    _mode = mode;
}

void TScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    // click on movable widget
    if (itemAt(mouseEvent->screenPos(), QTransform())
            && (itemAt(mouseEvent->screenPos(), QTransform())->type() == 3
            || itemAt(mouseEvent->screenPos(), QTransform())->type() == 12)) {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }

    if(_mode == Mode::DrawMode) {
        if (!_isLeftClickPressed) { // first press for line start point
            _isLeftClickPressed = true;
            _startPoint = mouseEvent->screenPos();

            QLabel *label = new QLabel();
            label->setGeometry(_startPoint.x(), _startPoint.y(), 50, 50);
            label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
            label->setStyleSheet("QLabel {color:blue;}");
            label->setWindowFlag(Qt::FramelessWindowHint); // No frame
            label->setAttribute(Qt::WA_NoSystemBackground); // No background
            label->setAttribute(Qt::WA_TranslucentBackground);
            QFont font = label->font();
            font.setPixelSize(13);
            label->setFont(font);
            addWidget(label);
            label->show();
            _labels.append(label);

            QLineF line;
            line.setP1(_startPoint);
            line.setP2(QPointF(_startPoint.x() + 1, _startPoint.y() + 1));
            _lineItem = new TLine(line);
            addItem(_lineItem);

        } else { // second press for line end point
            _isLeftClickPressed = false;
            _endPoint = mouseEvent->buttonDownScreenPos(Qt::LeftButton);
            if (_isShiftKeyPressed) { // shift key pressed then line end point change
                QLineF line;
                line.setP1(_startPoint);
                line.setP2(mouseEvent->buttonDownScreenPos(Qt::LeftButton));
                double angle = line.angle(); // in degree(between 0 to 360 ccw)

                QPointF point;
                if (angle <= 45.0 || angle >= 315.0 || (angle >= 135.0 && angle <= 225.0)) { // projected on x axis
                    point.setX(mouseEvent->buttonDownScreenPos(Qt::LeftButton).x());
                    point.setY(_startPoint.y());

                } else if ((angle > 35.0 && angle < 55.0) || (angle > 125.0 && angle < 145.0)
                           || (angle > 215.0 && angle < 235.0) || (angle > 305.0 && angle < 325.0)) { // projected on 45deg
                    point.setX(mouseEvent->screenPos().x());
                    double dx = line.dx();
                    angle = (angle > 35.0 && angle < 55.0) ? 45.0 : (angle > 125.0 && angle < 145.0)
                                                             ? 135.0 : (angle > 215.0 && angle < 235.0) ? 225.0 : 315.0;
                    double vatar = dx / qSin(angle * M_PI / 180.0);
                    int sign = (angle > 35.0 && angle < 55.0) ? -1 : (angle > 125.0 && angle < 145.0) ? -1 : 1;
                    double dy = sign * qSqrt(vatar * vatar - dx * dx);
                    point.setY(dy + line.y1());

                } else { // projected on y axis
                    point.setX(_startPoint.x());
                    point.setY(mouseEvent->buttonDownScreenPos(Qt::LeftButton).y());
                }
                QLineF newLine(_lineItem->line().p1(), point);
                _lineItem->setLine(newLine);
            } else {
                QLineF newLine(_lineItem->line().p1(), _endPoint);
                _lineItem->setLine(newLine);
            }
        }
    } else {
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
}

void TScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(_mode == Mode::DrawMode) {
        if (_isLeftClickPressed) { // check if mouse left button pressed for first time(line start point) or not
            if (_isShiftKeyPressed) { // check if shift key pressed or not(line drawing change to horizontal, vertical or 45deg according to the cursor position)
                QLineF line;
                line.setP1(_startPoint);
                line.setP2(mouseEvent->screenPos());
                double angle = line.angle(); // in degree(between 0 to 360 ccw)

                QPointF point;
                if (angle <= 35.0 || angle >= 325.0 || (angle >= 145.0 && angle <= 215.0)) { // projected on x axis
                    point.setX(mouseEvent->screenPos().x());
                    point.setY(_startPoint.y());

                } else if ((angle > 35.0 && angle < 55.0) || (angle > 125.0 && angle < 145.0)
                           || (angle > 215.0 && angle < 235.0) || (angle > 305.0 && angle < 325.0)) { // projected on 45deg
                    point.setX(mouseEvent->screenPos().x());
                    double dx = line.dx();
                    angle = (angle > 35.0 && angle < 55.0) ? 45.0 : (angle > 125.0 && angle < 145.0)
                                                             ? 135.0 : (angle > 215.0 && angle < 235.0) ? 225.0 : 315.0;
                    double vatar = dx / qSin(angle * M_PI / 180.0);
                    int sign = (angle > 35.0 && angle < 55.0) ? -1 : (angle > 125.0 && angle < 145.0) ? -1 : 1;
                    double dy = sign * qSqrt(vatar * vatar - dx * dx);
                    point.setY(dy + line.y1());

                } else { // projected on y axis
                    point.setX(_startPoint.x());
                    point.setY(mouseEvent->screenPos().y());

                }
                QLineF newLine(_lineItem->line().p1(), point);
                _lineItem->setLine(newLine);

            } else {
                QLineF newLine(_lineItem->line().p1(), mouseEvent->screenPos());
                _lineItem->setLine(newLine);
            }
            _labels.last()->setText(QString::number(_lineItem->line().length()));
            _labels.last()->setGeometry(_lineItem->line().center().x(),
                                        _lineItem->line().center().y(), 50, 50);
        }
    } else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void TScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Shift)
        _isShiftKeyPressed = true;
    else if (keyEvent->key() == Qt::Key_Delete)
        foreach (QGraphicsItem *i, selectedItems()) removeItem(i);
}

void TScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Shift)
        _isShiftKeyPressed = false;
}

void TScene::handle_btnClear_click()
{
    if (_labels.count() && _lineItem != nullptr && !_isLeftClickPressed) {
        qDeleteAll(_labels);
        _labels.clear();

        QList<QGraphicsItem*> tempItems = items();
        QList<QGraphicsItem*> lineItems;
        for (int i = 0; i < tempItems.count(); ++i) {
            if (items().at(i)->type() == QGraphicsItem::UserType + 1) {
                lineItems.append(items().at(i));
            }
        }
        foreach (QGraphicsItem *i, lineItems) removeItem(i);
        _lineItem = nullptr;
    }
}