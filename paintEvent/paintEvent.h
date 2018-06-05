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

        QPixmap pixmap(size());
        pixmap.fill(Qt::white);
        QPainter p(&pixmap);

        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(Qt::black, 10));
        p.drawRect(rect);

        setPixmap(pixmap);
        repaint(rect.adjusted(-5,-5,5,5)); // adjusted to include stroke
    }

private:
    QRect rect;
    QPoint startPt;
};