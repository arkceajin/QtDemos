/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by arkceajin@gmail.com                             *
 *                                                                         *
 *   MIT License                                                           *
 *                                                                         *
 ***************************************************************************/
#ifndef QTOPENCVCAM_H
#define QTOPENCVCAM_H

#include <QObject>
#include <QAbstractVideoSurface>
#include "opencv2/core/core.hpp"
#include "opencv2/videoio/videoio.hpp"

class QtOpenCVCam : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface \
               WRITE setVideoSurface NOTIFY videoSurfaceChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
public:
    explicit QtOpenCVCam(int index = 0, QObject *parent = nullptr);
    ~QtOpenCVCam();

    QAbstractVideoSurface *videoSurface() const;
    void setVideoSurface(QAbstractVideoSurface *videoSurface);

    QSize size() const;

signals:
    void videoSurfaceChanged();
    void captured(const QImage &frame);
    void sizeChanged();

public slots:
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
private slots:
    void renderFrame(const QImage &frame);
private:
    cv::VideoCapture        mCap;
    QThread*                mStream;
    QAbstractVideoSurface*  mVideoSurface;
    QSize                   mSize;
    void setSize(const QSize &size);
};

#endif // QTOPENCVCAM_H
