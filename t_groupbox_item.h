#ifndef T_GROUPBOX_ITEM_H
#define T_GROUPBOX_ITEM_H

#include <QGraphicsItemGroup>

class QGraphicsRectItem;
class QGraphicsProxyWidget;

class TMovableWidgetItem : public QGraphicsItemGroup
{
public:
    TMovableWidgetItem(QWidget *widget, QGraphicsItem *parent = nullptr);

private:
    QGraphicsRectItem *_rect;
    QWidget *_widget;
    QColor _color;
    QGraphicsProxyWidget *_proxy;
};

#endif // T_GROUPBOX_ITEM_H
