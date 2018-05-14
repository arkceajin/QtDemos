#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QApplication>
#include <QLabel>
#include <QDesktopWidget>
#include <QScreen>
#include <QKeyEvent>
#include <QTimer>

class ScreenCapture : public QLabel
{
    Q_OBJECT
public:
    explicit ScreenCapture(const WId& wid = 0,
                           const int& interval = 1000,
                           QWidget* parent = Q_NULLPTR) :
        QLabel(parent),
        mWId(wid),
        mInterval(interval)
    {
        if(parent == Q_NULLPTR)
            resize(QApplication::desktop()->screenGeometry().size() * 0.5);

        setScaledContents(true);
        connect(&mTimer, &QTimer::timeout, [=](){
            QWidget* w = QApplication::activeWindow();
            if(w != Q_NULLPTR)
                w->setWindowOpacity(0);
            mBuffer = QApplication::primaryScreen()->grabWindow(mWId);
            if(w != Q_NULLPTR)
                w->setWindowOpacity(1);
            setPixmap(mBuffer);
        });
        start();
    }

public slots:
    inline void start(){
        mTimer.start(mInterval);
    }

    inline void stop(){
        mTimer.stop();
        clear();
    }

    inline bool save(const QString& name = "screen.png"){
        return mBuffer.save(name);
    }

protected:
    void keyPressEvent(QKeyEvent *ev) override {
        if(ev->key() == Qt::Key_S &&
           ev->modifiers() & Qt::AltModifier){
            save();
        }
    }

private:
    const WId   mWId;
    const int   mInterval;
    QTimer      mTimer;
    QPixmap     mBuffer;
};

#endif // SCREENCAPTURE_H
