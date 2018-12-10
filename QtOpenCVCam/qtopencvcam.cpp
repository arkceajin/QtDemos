/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by arkceajin@gmail.com                             *
 *                                                                         *
 *   MIT License                                                           *
 *                                                                         *
 ***************************************************************************/
#include "qtopencvcam.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QThread>
#include <QVideoSurfaceFormat>
#include <QDebug>

QImage mat2QImage(const cv::Mat& mat)
{
    cv::Mat temp;
    cv::cvtColor(mat, temp, cv::COLOR_RGB2BGR);
    return QImage(static_cast<const uchar*>(temp.data),
                  mat.cols, mat.rows, mat.step,
                  QImage::Format_RGB888).copy();
}

class StreamProcess: public QThread
{
    Q_OBJECT
public:
    explicit StreamProcess(cv::VideoCapture* cap, QObject* parent = nullptr) :
        QThread(parent),
        mCap(cap)
    {}
protected:
    void run() override{
        while(true) {
            if(!mCap->isOpened())
                continue;
            cv::Mat frame;
            (*mCap) >> frame;
            emit captured(mat2QImage(frame));
        }
    }
signals:
    void captured(const QImage& frame);
private:
    cv::VideoCapture* mCap;
};

#include "qtopencvcam.moc"

QtOpenCVCam::QtOpenCVCam(QObject *parent) :
    QObject(parent),
    mCap(0),
    mStream(new StreamProcess(&mCap, this)),
    mVideoSurface(nullptr),
    mSize()
{
    connect(qobject_cast<StreamProcess*>(mStream), &StreamProcess::captured,
            this, &QtOpenCVCam::renderFrame, Qt::QueuedConnection);
}

QtOpenCVCam::~QtOpenCVCam()
{
    stop();
}

QAbstractVideoSurface *QtOpenCVCam::videoSurface() const
{
    return mVideoSurface;
}

void QtOpenCVCam::setVideoSurface(QAbstractVideoSurface *videoSurface)
{
    if(videoSurface == mVideoSurface)
        return;
    mVideoSurface = videoSurface;

    if(mVideoSurface != nullptr)
        start();

    emit videoSurfaceChanged();
}

void QtOpenCVCam::start()
{
    if(mStream->isRunning())
        return;
    mStream->start();

    if(!mVideoSurface)
        return;
    mSize.setWidth(mCap.get(cv::CAP_PROP_FRAME_WIDTH));
    mSize.setHeight(mCap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const QVideoFrame::PixelFormat& pixelFormat = QVideoFrame::pixelFormatFromImageFormat(QImage::Format_RGB32);
    mVideoSurface->start(QVideoSurfaceFormat(mSize, pixelFormat));
}

void QtOpenCVCam::stop()
{
    mSize = QSize();
    mStream->terminate();
    mStream->wait();

    if(mVideoSurface)
        mVideoSurface->stop();
}

void QtOpenCVCam::renderFrame(const QImage &frame)
{
    if(mVideoSurface)
        mVideoSurface->present(QVideoFrame(frame));
    emit captured(frame);
}

