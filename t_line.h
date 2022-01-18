#ifndef T_LINE_H
#define T_LINE_H

#include <QGraphicsLineItem>

class TLine : public QGraphicsLineItem
{
public:
    TLine(QPointF startPoint, QPointF endPoint,
          QGraphicsItem *parent = nullptr);
    TLine(QLineF line,
          QGraphicsItem *parent = nullptr);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QPointF startPoint() const { return _startPoint; }
    QPointF endPoint() const { return _endPoint; }

    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;

private:
    QPointF _startPoint;
    QPointF _endPoint;

    QColor _color;
};

#endif // T_LINE_H
