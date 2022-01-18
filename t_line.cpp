#include "t_line.h"

#include <QPainter>

#include <QDebug>

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

TLineLabelItem::TLineLabelItem(QPointF startPoint, QPointF endPoint, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    setHandlesChildEvents(false);

    _startPoint = startPoint;
    _endPoint   = endPoint;
    _color      = Qt::black;
    _colorSelected = Qt::red;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _line = new QGraphicsLineItem(this);
    _line->setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _line->setLine(_startPoint.x(), _startPoint.y(), _endPoint.x(), _endPoint.y());
    _line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _label = new QGraphicsTextItem();

    addToGroup(_line);
    addToGroup(_label);
}

TLineLabelItem::TLineLabelItem(QLineF line, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    _startPoint = line.p1();
    _endPoint   = line.p2();
    _color      = Qt::black;
    _colorSelected = Qt::red;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _line = new QGraphicsLineItem(this);
    _line->setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _line->setLine(line);
    _line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _label = new QGraphicsTextItem();

    addToGroup(_line);
    addToGroup(_label);
}

QGraphicsLineItem *TLineLabelItem::getLine()
{
    return _line;
}

QGraphicsTextItem *TLineLabelItem::getLabel()
{
    return _label;
}

QRectF TLineLabelItem::boundingRect() const
{
    qreal extra = (_line->pen().width() + 5) / 2.0;

//    return QRectF(_line->line().p1(), QSizeF(_line->line().p2().x() - _line->line().p1().x(),
//                                             _line->line().p2().y() - _line->line().p1().y()))
//            .normalized()
//            .adjusted(-extra, -extra, extra, extra);
    return QRectF(_line->line().p1(), _line->line().p2())
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void TLineLabelItem::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen myPen = _line->pen();
    myPen.setColor(_color);
    painter->setPen(myPen);
    painter->setBrush(_color);
    painter->drawLine(_line->line());

    if (isSelected()) {
        painter->setPen(QPen(_colorSelected, 4));
        QLineF myLine = _line->line();
        painter->drawLine(myLine);
    }
}
