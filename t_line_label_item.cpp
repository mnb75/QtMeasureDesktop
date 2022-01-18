#include "t_line_label_item.h"

#include <QPainter>

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

TLineLabelItem::TLineLabelItem(QLineF line, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    _startPoint = line.p1();
    _endPoint   = line.p2();
    _lineColor  = Qt::black;
    _selectedLineColor = Qt::red;
    _labelColor = Qt::black;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _line = new QGraphicsLineItem(this);
    _line->setPen(QPen(_lineColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _line->setLine(line);
    _line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _label = new QGraphicsTextItem();
    _label->setDefaultTextColor(_labelColor);

    addToGroup(_line);
    addToGroup(_label);
}

TLineLabelItem::TLineLabelItem(TLineLabelItem *lineLabelItem)
{
    QLineF line = lineLabelItem->getLine()->line();
    line.translate(20, 20);

    _startPoint = line.p1();
    _endPoint   = line.p2();
    _lineColor  = Qt::black;
    _selectedLineColor = Qt::red;
    _labelColor = Qt::black;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _line = new QGraphicsLineItem(this);
    _line->setPen(QPen(_lineColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _line->setLine(line);
    _line->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);

    _label = new QGraphicsTextItem();
    _label->setPlainText(lineLabelItem->getLabel()->toPlainText());
    _label->setPos(line.center());
    _label->setDefaultTextColor(_labelColor);

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

void TLineLabelItem::setLineColor(QColor color)
{
    _lineColor = color;
    QPen pen = _line->pen();
    pen.setColor(_lineColor);
    _line->setPen(pen);
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
    myPen.setColor(_lineColor);
    painter->setPen(myPen);
    painter->setBrush(_lineColor);
    painter->drawLine(_line->line());

    if (isSelected()) {
        painter->setPen(QPen(_selectedLineColor, 5, Qt::DashLine));
        QLineF myLine = _line->line();
        painter->drawLine(myLine);
    }
}
