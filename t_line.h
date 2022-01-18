#ifndef T_LINE_H
#define T_LINE_H

#include <QGraphicsItemGroup>

class TLineLabelItem : public QGraphicsItemGroup
{
public:
    TLineLabelItem(QPointF startPoint, QPointF endPoint,
          QGraphicsItem *parent = nullptr);
    TLineLabelItem(QLineF line,
          QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QPointF startPoint() const { return _startPoint; }
    QPointF endPoint() const { return _endPoint; }

    QGraphicsLineItem *getLine();
    QGraphicsTextItem *getLabel();

    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;

private:
    QPointF _startPoint;
    QPointF _endPoint;

    QColor _color;
    QColor _colorSelected;

    QGraphicsLineItem *_line;
    QGraphicsTextItem *_label;
};

#endif // T_LINE_H
