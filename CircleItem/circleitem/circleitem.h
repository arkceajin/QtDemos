#ifndef GAUGEITEM_H
#define GAUGEITEM_H

#include <QObject>
#include <QQuickItem>
#include <QSGGeometry>

/**
 * @brief The CircleItem class
 * Customize QQuickItem for circle sweeping animation
 */
class CircleItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float sweepAngle READ sweepAngle WRITE setSweepAngle NOTIFY sweepAngleChanged)
    Q_PROPERTY(float startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(bool clockwise READ clockwise WRITE setClockwise NOTIFY clockwiseChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QPointF radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QPointF center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(int vertex READ vertex WRITE setVertex NOTIFY vertexChanged)
public:
    CircleItem();

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

    static void registerType();

    float sweepAngle() const;
    void setSweepAngle(const float &sweepAngle);

    float startAngle() const;
    void setStartAngle(const float &startAngle);

    bool clockwise() const;
    void setClockwise(bool clockwise);

    float lineWidth() const;
    void setLineWidth(float lineWidth);

    QColor color() const;
    void setColor(const QColor &color);

    int vertex() const;
    void setVertex(int vertex);

    QPointF center() const;
    void setCenter(const QPointF &center);

    QPointF radius() const;
    void setRadius(const QPointF &radius);

signals:
    void sweepAngleChanged();
    void startAngleChanged();
    void clockwiseChanged();
    void lineWidthChanged();
    void colorChanged();
    void radiusChanged();
    void centerChanged();
    void vertexChanged();

private:
    float m_sweepAngle = 0;
    float m_startAngle = 0;
    float m_lineWidth = 1;
    QPointF m_radius = QPointF(0, 0);
    QPointF m_center = QPointF(0, 0);
    bool m_clockwise = true;
    bool m_updateVertex = false;
    QColor m_color = Qt::black;
    QVector<QSGGeometry::Point2D> m_vertexBuffer;

    /**
     * @brief updateVertexData
     * update vertex only when fellowing property
     * 1. startAngle
     * 2. radius
     * 3. center
     * 4. clockwise
     */
    void updateVertexData();
};

#endif // GAUGEITEM_H
