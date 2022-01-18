#include "t_movable_widget_item.h"

#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QWidget>
#include <QPen>

TMovableWidgetItem::TMovableWidgetItem(QWidget *widget, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent), _widget(widget)
{
    setHandlesChildEvents(false); // each children handles its events

    setFlags(QGraphicsItem::ItemIsMovable);
    setZValue(1000);

    _color = Qt::gray;

    _rect = new QGraphicsRectItem(this);
    _rect->setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    _rect->setRect(0, 0, _widget->rect().width(), 25);
    _rect->setBrush(_color);

    _proxy = new QGraphicsProxyWidget(this);
    _proxy->setWidget(_widget);

    addToGroup(_rect);
    addToGroup(_proxy);
}
