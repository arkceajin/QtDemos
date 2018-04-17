#ifndef COLORIZEPROGRESSBAR_H
#define COLORIZEPROGRESSBAR_H

#include <QProgressBar>
#include <QGraphicsEffect>
#include <QPainter>

class ColorizeProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit ColorizeProgressBar(QWidget *parent = Q_NULLPTR) :
        QProgressBar(parent),
        mColorize(new Colorize)
    {
        setGraphicsEffect(mColorize);
        connect(this, & QProgressBar::valueChanged, [=](int value){
            mColorize->percent = (float)value / (maximum() - minimum());
        });
    }

    void setColor(const QColor& color) {
        mColorize->color = color;
    }

    void setStrength(const qreal& strength) {
        mColorize->strength = strength;
    }

private:
    class Colorize : public QGraphicsEffect {
    public:
        explicit Colorize() :
            QGraphicsEffect(),
            strength(1),
            color(Qt::red),
            margins(0.5, 0.5, 30, 0.5) // have to adjust base on OS
        { }
        quint32 strength;
        QColor color;
        float percent;

    protected:
        void draw(QPainter* painter) {
            QPoint offset;
            const QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
            draw(painter, offset, pixmap, QRect());
        }

        void draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect) const
        {
            if (src.isNull())
                return;

            QImage srcImage;
            QImage destImage;

            if (srcRect.isNull()) {
                srcImage = src.toImage();
                srcImage = srcImage.convertToFormat(srcImage.hasAlphaChannel() ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32);
                destImage = QImage(srcImage.size(), srcImage.format());
            } else {
                QRect rect = srcRect.toAlignedRect().intersected(src.rect());

                srcImage = src.copy(rect).toImage();
                srcImage = srcImage.convertToFormat(srcImage.hasAlphaChannel() ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32);
                destImage = QImage(rect.size(), srcImage.format());
            }
            destImage.setDevicePixelRatio(src.devicePixelRatioF());

            // do colorizing
            QPainter destPainter(&destImage);
            grayscale(srcImage, destImage, srcImage.rect());
            destPainter.setCompositionMode(QPainter::CompositionMode_Screen);
            QRectF rect = QRectF(srcImage.rect()).marginsRemoved(margins);
            rect.setWidth(rect.width() * percent);
            destPainter.fillRect(rect, color);
            destPainter.end();

            // alpha blending srcImage and destImage
            if(0 < strength && strength < 1){
                QImage buffer = srcImage;
                QPainter bufPainter(&buffer);
                bufPainter.setOpacity(strength);
                bufPainter.drawImage(0, 0, destImage);
                bufPainter.end();
                destImage = buffer;
            }

            if (srcImage.hasAlphaChannel())
                destImage.setAlphaChannel(srcImage.alphaChannel());

            painter->drawImage(dest, destImage);
        }
    private:
        const QMarginsF margins;
    };
private:
    Colorize* mColorize;

    static void grayscale(const QImage &image, QImage &dest, const QRect& rect = QRect())
    {
        QRect destRect = rect;
        QRect srcRect = rect;
        if (rect.isNull()) {
            srcRect = dest.rect();
            destRect = dest.rect();
        }
        if (&image != &dest) {
            destRect.moveTo(QPoint(0, 0));
        }

        const unsigned int *data = (const unsigned int *)image.bits();
        unsigned int *outData = (unsigned int *)dest.bits();

        if (dest.size() == image.size() && image.rect() == srcRect) {
            // a bit faster loop for grayscaling everything
            int pixels = dest.width() * dest.height();
            for (int i = 0; i < pixels; ++i) {
                int val = qGray(data[i]);
                outData[i] = qRgba(val, val, val, qAlpha(data[i]));
            }
        } else {
            int yd = destRect.top();
            for (int y = srcRect.top(); y <= srcRect.bottom() && y < image.height(); y++) {
                data = (const unsigned int*)image.scanLine(y);
                outData = (unsigned int*)dest.scanLine(yd++);
                int xd = destRect.left();
                for (int x = srcRect.left(); x <= srcRect.right() && x < image.width(); x++) {
                    int val = qGray(data[x]);
                    outData[xd++] = qRgba(val, val, val, qAlpha(data[x]));
                }
            }
        }
    }
};
#endif // COLORIZEPROGRESSBAR_H
