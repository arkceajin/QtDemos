#include <QPainter>
#include <QPaintEvent>
#include <QWidget>

class Widget : public QLabel
{
public:
    using QLabel::QLabel;

    void mousePressEvent(QMouseEvent *e) override
    {
        startPt = e->pos();
        rect = QRect();
        update(); // clear the entire widget
    }

    void mouseMoveEvent(QMouseEvent *e) override
    {
        rect = QRect(startPt, e->pos()).normalized();
        qDebug()<<"start paint";
        repaint(rect.adjusted(-5,-5,5,5)); // adjusted to include stroke
        grab().save(QString::number(++i) + ".PNG", "PNG");
        qDebug()<<"end paint";
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);

        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(Qt::black, 10));
        p.drawRect(rect);
    }

private:
    QRect rect;
    QPoint startPt;
    int i = 0;
};
