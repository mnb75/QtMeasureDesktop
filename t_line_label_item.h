#ifndef T_LINE_H
#define T_LINE_H

#include <QGraphicsItemGroup>

class TLineLabelItem : public QGraphicsItemGroup
{
public:
    TLineLabelItem(QLineF line, QGraphicsItem *parent = nullptr);
    TLineLabelItem(TLineLabelItem *lineLabelItem); // copy constructor

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QGraphicsLineItem *getLine();
    QGraphicsTextItem *getLabel();

    void setLineColor(QColor color);

    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0) override;

private:
    QPointF _startPoint;
    QPointF _endPoint;

    QColor _lineColor;
    QColor _selectedLineColor;
    QColor _labelColor;

    QGraphicsLineItem *_line;
    QGraphicsTextItem *_label;
};

#endif // T_LINE_H
