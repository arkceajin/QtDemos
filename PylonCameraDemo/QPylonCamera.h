#ifndef QPYLONCAMERA_H
#define QPYLONCAMERA_H

#include <QObject>
#include <QAbstractVideoSurface>

#if _MSC_VER

#include <pylon/PylonIncludes.h>

using namespace Pylon;

/**
 * @brief The QPylonImageEventHandler class
 * Handler the grabbed frame and convert it to QImage
 */
class QPylonImageEventHandler : public QObject, public CImageEventHandler
{
    Q_OBJECT
public:
    using QObject::QObject;

    /**
     * @brief OnImageGrabbed
     * @param camera
     * @param ptrGrabResult
     */
    virtual void OnImageGrabbed(CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult) override;

signals:
    void frameGrabbed(const QImage &frame);

private:
    static QImage toQImage(const CPylonImage &pylonImage);
};

/**
 * @brief The QPylonCamera class
 */
class QPylonCamera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface \
               WRITE setVideoSurface NOTIFY videoSurfaceChanged)
public:
    explicit QPylonCamera(QObject *parent = Q_NULLPTR);
    virtual ~QPylonCamera();

    inline bool isOpen() const {
        return mCamera != Q_NULLPTR && mCamera->IsOpen();
    }

    QAbstractVideoSurface *videoSurface() const;
    void setVideoSurface(QAbstractVideoSurface *videoSurface);

signals:
    void videoSurfaceChanged();
    void captured(const QImage &imgs);

public slots:
    Q_INVOKABLE bool start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void capture();

private slots:
    /**
     * @brief renderFrame
     * Render the grabbed frame for the video surface
     * @param frame
     */
    void renderFrame(const QImage &frame);
private:
    /**
     * @brief open
     * Open camera device
     */
    void open();
    /**
     * @brief startGrabbing
     * Start collect frame from camera
     * And establish the connection between ImageHandler & VideoSurface
     * @return
     */
    bool startGrabbing();
    /**
     * @brief stopGrabbing
     */
    void stopGrabbing();

    QPylonImageEventHandler*mImageEventHandler;
    QAbstractVideoSurface*  mVideoSurface;
    CInstantCamera*         mCamera;
    QRect                   mRect;
    QImage                  mBuffer;
};
#else
//If you find the way to use Pylon with Mingw, Please contact me.

#include <QCamera>
#include <QCameraImageCapture>
class Camera : public QCamera
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface \
               WRITE setVideoSurface NOTIFY videoSurfaceChanged)
public:
    Camera() {
        imageCapture = new QCameraImageCapture(this);
        connect(imageCapture, &QCameraImageCapture::imageCaptured,
                this, &Camera::captured, Qt::QueuedConnection);
    }
    virtual ~Camera() {}

    Q_INVOKABLE void capture() {
        qDebug()<<"capture"<<imageCapture->capture();
    }

    QAbstractVideoSurface *videoSurface() const {
        return m_surface;
    }

    void setVideoSurface(QAbstractVideoSurface *surface) {
        if(m_surface != surface) {
            setViewfinder(surface);
            m_surface = surface;
            stop();
            start();
            emit videoSurfaceChanged();
        }
    }
signals:
    void videoSurfaceChanged();
    void captured(int id, const QImage &img);
private:
    QAbstractVideoSurface *m_surface;
    QCameraImageCapture* imageCapture;
};
#endif

#endif // QPYLONCAMERA_H
