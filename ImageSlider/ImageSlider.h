#ifndef ImageSlider_H
#define ImageSlider_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QResizeEvent>
#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>
#include <QTimer>
#include <QGraphicsSceneHoverEvent>
#include <QDebug>

#define PreviewRatio    0.2
#define DefaultInterval 1000

/**
 * @brief The ImageSlider class
 */
class ImageSlider : public QGraphicsView
{
    Q_OBJECT
public:
    ImageSlider(const QStringList& imgs,
                const int& interval = 1000,
                QWidget* parent = 0):
        QGraphicsView(parent),
        mScene(new QGraphicsScene(this)),
        mSlider(new Slider()),
        mTimer(new QTimer(this)),
        mInterval(interval)
    {
        setScene(mScene);
        setBackgroundBrush(Qt::black);
        mScene->addItem(mSlider);

        if(parent == 0) // if it's top-level window, shows at centre of the desktop
            setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    size(),
                    qApp->desktop()->availableGeometry()
                )
            );

        connect(mTimer, &QTimer::timeout, [=](){
            int next = mSlider->index() + 1;
            if(next == mSlider->count())
                next = 0;
            mSlider->showImage(next);
        });

        foreach (QString path, imgs) {
            mScene->addItem(mSlider->appendImage(path));
        }
    }

    int interval() const {
        return mInterval;
    }

    void setInterval(const int& interval) {
        mInterval = interval;
    }

protected:
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE{
        const QSize& size = event->size();
        mScene->setSceneRect(0, 0, size.width(), size.height());
        mSlider->updateSlider(size);
        QGraphicsView::event(event);
    }

    void showEvent(QShowEvent* event) Q_DECL_OVERRIDE{
        mSlider->showImage(0);
        if(mInterval != 0)
            mTimer->start(mInterval);
        QGraphicsView::showEvent(event);
    }

    void hideEvent(QHideEvent* event) Q_DECL_OVERRIDE{
        mTimer->stop();
        QGraphicsView::hideEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE {
        mTimer->stop();
        QGraphicsView::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE {
        mTimer->start(mInterval);
        QGraphicsView::mouseReleaseEvent(event);
    }

private:
    /**
     * @brief The Slider class
     * 1. display the process and the preview viewport
     * 2. store the frames.
     * 3. listen to the hover event to show the preview
     * 4. listen to the mouse press event to jump the certain time point.
     */
    class Slider: public QGraphicsRectItem{
    public:
        Slider(QGraphicsItem *parent = Q_NULLPTR):
            QGraphicsRectItem(parent),
            mSliderBar(new QGraphicsRectItem(this)),
            mPreview(new QGraphicsPixmapItem(this)),
            mLine(new QGraphicsLineItem(this)),
            mImages(),
            mIndex(0)
        {
            setAcceptHoverEvents(true);
            setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
            setOpacity(0.5);
            setBrush(Qt::gray);
            setZValue(1);
            setPen(Qt::NoPen);
            mSliderBar->setBrush(Qt::darkGray);
            mSliderBar->setPen(Qt::NoPen);
        }

        void updateSlider(const QSize& size){
            qreal sliderHeight = 0.05 * size.height();
            setRect(0, 0, size.width(), sliderHeight);
            setPos(0, 0.85 * size.height());
            foreach(Image img, mImages){
                QGraphicsPixmapItem * item = img.item;
                QPixmap* pixmap = img.pixmap;
                item->setPixmap(pixmap->scaled(size, Qt::IgnoreAspectRatio));
            }
            updateSliderBar();
        }

        void updateSliderBar(){
            qreal percentage = (qreal)(mIndex + 1) / count();
            qreal sliderWidth = percentage * rect().width();
            mSliderBar->setRect(0, 0, sliderWidth, rect().height());
            mSliderBar->setPos(0, 0);
        }

        void updatePreview(QPixmap* p){
            if(mPreview->isVisible())
                mPreview->setPixmap(*p);
        }

        QGraphicsPixmapItem* appendImage(const QString& imagePath){
            QPixmap* pixmap = new QPixmap(imagePath);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(*pixmap);
            item->hide();
            mImages.push_back({item, pixmap});
            return item;
        }

        QPixmap* getPixmap(const int& i){
            if(i < 0 || i >= count())
                return false;
            return mImages[i].pixmap;
        }

        inline int count(){
            return mImages.count();
        }

        void showImage(const int& i){
            if(i < 0 || i >= count())
                return;
            mImages[mIndex].item->hide();
            mImages[i].item->show();
            mIndex = i;
            updateSliderBar();
        }

        int index() const{
            return mIndex;
        }

    protected:
        void hoverEnterEvent(QGraphicsSceneHoverEvent* event){
            mPreview->show();
            mLine->show();
            QGraphicsRectItem::hoverEnterEvent(event);
        }

        void hoverMoveEvent(QGraphicsSceneHoverEvent* event){
            const QPixmap& pixmap = mImages[xToIndex(event->pos().x())]. \
                    pixmap->scaledToWidth(rect().width() * PreviewRatio);
            mPreview->setPixmap(pixmap);
            mPreview->setPos(event->pos().x() - pixmap.width() / 2,
                             -(pixmap.height() + rect().height()));
            mLine->setLine(event->pos().x(), 0, event->pos().x(), rect().height());
            QGraphicsRectItem::hoverMoveEvent(event);
        }

        void hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
            mPreview->hide();
            mLine->hide();
            QGraphicsRectItem::hoverLeaveEvent(event);
        }

        void mousePressEvent(QGraphicsSceneMouseEvent* event){
            showImage(xToIndex(event->pos().x()));
            QGraphicsRectItem::mousePressEvent(event);
        }

    private:
        struct Image{
            QGraphicsPixmapItem*    item;
            QPixmap*                pixmap;
        };
        QGraphicsRectItem*      mSliderBar;
        QGraphicsPixmapItem*    mPreview;
        QGraphicsLineItem*      mLine;
        QVector<Image>          mImages;
        int                     mIndex;

        inline int xToIndex(int x){
            qreal p = x / rect().width();
            return p * count();
        }
    };

    QGraphicsScene*     mScene;
    Slider*             mSlider;
    QTimer*             mTimer;
    int                 mInterval;
};


#endif // ImageSlider_H
