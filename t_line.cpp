#include "t_line.h"

#include <QPainter>

#include <QDebug>

TLine::TLine(QPointF startPoint, QPointF endPoint, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    _startPoint = startPoint;
    _endPoint   = endPoint;
    _color      = Qt::black;
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setLine(_startPoint.x(), _startPoint.y(), _endPoint.x(), _endPoint.y());
}

TLine::TLine(QLineF line, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    _startPoint = line.p1();
    _endPoint   = line.p2();
    _color      = Qt::black;
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setLine(line);
}

QRectF TLine::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void TLine::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
//qDebug() << Q_FUNC_INFO << "press" << __LINE__;
    QPen myPen = pen();
    myPen.setColor(_color);
    painter->setPen(myPen);
    painter->setBrush(_color);

//    setLine(QLineF(_startPoint, _endPoint));

    painter->drawLine(line());
    if (isSelected()) {
        painter->setPen(QPen(_color, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
