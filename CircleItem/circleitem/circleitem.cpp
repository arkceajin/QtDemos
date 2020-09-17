#include "circleitem.h"
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <cmath>

CircleItem::CircleItem()
{
    setFlag(ItemHasContents);
}

QSGNode *CircleItem::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *node = nullptr;
    QSGGeometry *geometry = nullptr;
    QSGFlatColorMaterial *material = nullptr;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setLineWidth(lineWidth());
        geometry->setDrawingMode(QSGGeometry::DrawPoints);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        material = new QSGFlatColorMaterial;
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        material = static_cast<QSGFlatColorMaterial *>(node->material());
    }
    material->setColor(color());

    if (m_updateVertex) {
        updateVertexData();
        m_updateVertex = false;
    }

    const int& countPoints = static_cast<int>((sweepAngle() / 360.0) * vertex());
    geometry->allocate(countPoints, 0);
    std::copy(m_vertexBuffer.begin(), m_vertexBuffer.begin() + countPoints, geometry->vertexDataAsPoint2D());

    // Notify all the drawers of the node geometry change
    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    return node;
}

void CircleItem::registerType()
{
    qmlRegisterType<CircleItem>("CircleItem", 1, 0, "CircleItem");
}

float CircleItem::sweepAngle() const
{
    return m_sweepAngle;
}

void CircleItem::setSweepAngle(const float &sweepAngle)
{
    if (std::isnan(sweepAngle))
        return;
    m_sweepAngle = fmod(sweepAngle, 360);
    if (m_sweepAngle < 0)
        m_sweepAngle += 360;
    update();
    emit sweepAngleChanged();
}

float CircleItem::startAngle() const
{
    return m_startAngle;
}

void CircleItem::setStartAngle(const float &startAngle)
{
    if (std::isnan(startAngle))
        return;
    m_startAngle = fmod(startAngle, 360);
    if (m_startAngle < 0)
        m_startAngle += 360;
    m_updateVertex = true;
    update();
    emit startAngleChanged();
}

bool CircleItem::clockwise() const
{
    return m_clockwise;
}

void CircleItem::setClockwise(bool clockwise)
{
    m_clockwise = clockwise;
    m_updateVertex = true;
    update();
    emit clockwiseChanged();
}

float CircleItem::lineWidth() const
{
    return m_lineWidth;
}

void CircleItem::setLineWidth(float lineWidth)
{
    m_lineWidth = lineWidth;
    update();
    emit lineWidthChanged();
}

QColor CircleItem::color() const
{
    return m_color;
}

void CircleItem::setColor(const QColor &color)
{
    m_color = color;
    update();
    emit colorChanged();
}

int CircleItem::vertex() const
{
    return m_vertexBuffer.size();
}

void CircleItem::setVertex(int vertex)
{
    m_vertexBuffer.resize(vertex);
    m_updateVertex = true;
    update();
    emit vertexChanged();
}

QPointF CircleItem::center() const
{
    return m_center;
}

void CircleItem::setCenter(const QPointF &center)
{
    m_center = center;
    m_updateVertex = true;
    update();
    emit centerChanged();
}

QPointF CircleItem::radius() const
{
    return m_radius;
}

void CircleItem::setRadius(const QPointF &radius)
{
    m_radius = radius;
    m_updateVertex = true;
    update();
    emit radiusChanged();
}

void CircleItem::updateVertexData()
{
    for (int i = 0; i < vertex(); ++i) {
        const float& rad = (((float)i / vertex()) * 360.0 - m_startAngle) * (M_PI / 180.0) * (m_clockwise? -1: 1);
        m_vertexBuffer[i].set(std::sin(rad) * m_radius.x() + m_center.x(), std::cos(rad) * m_radius.y() + m_center.y());
    }
}
