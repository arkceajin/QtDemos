#ifndef GAUGEITEM_H
#define GAUGEITEM_H

#include <QObject>
#include <QQuickItem>
#include <QSGGeometry>

class CircleItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(float startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(bool clockwise READ clockwise WRITE setClockwise NOTIFY clockwiseChanged)
    Q_PROPERTY(float lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(int vertex READ vertex WRITE setVertex NOTIFY vertexChanged)
public:
    CircleItem();

    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

    static void registerType();

    float angle() const;
    void setAngle(const float &angle);

    float startAngle() const;
    void setStartAngle(const float &startAngle);

    bool clockwise() const;
    void setClockwise(bool clockwise);

    float lineWidth() const;
    void setLineWidth(float lineWidth);

    QColor color() const;
    void setColor(const QColor &color);

    float radius() const;
    void setRadius(float radius);

    int vertex() const;
    void setVertex(int vertex);

signals:
    void angleChanged();
    void startAngleChanged();
    void clockwiseChanged();
    void lineWidthChanged();
    void colorChanged();
    void radiusChanged();
    void vertexChanged();

private:
    float m_angle = 0;
    float m_startAngle = 0;
    float m_lineWidth = 1;
    float m_radius = 0;
    bool m_clockwise = true;
    bool m_updateVertex = false;
    QColor m_color = Qt::black;
    QVector<QSGGeometry::Point2D> m_vertexBuffer;

    void updateVertexData();
};

#endif // GAUGEITEM_H
