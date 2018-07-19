#include "QPylonCamera.h"
#include <QDebug>
#include <QVideoSurfaceFormat>
#include <QGuiApplication>

// Namespace for using GenApi objects.
using namespace GenApi;

void QPylonImageEventHandler::OnImageGrabbed(CInstantCamera &camera, const CGrabResultPtr &ptrGrabResult)
{
    Q_UNUSED(camera)

    if(!ptrGrabResult.IsValid()) {
        qWarning()<<"Invalid frame grabbed";
        return;
    }

    CImageFormatConverter fc;
    fc.OutputPixelFormat = PixelType_RGB8packed;

    CPylonImage pylonImage;

    fc.Convert(pylonImage, ptrGrabResult);
    if (!pylonImage.IsValid()) {
        qWarning()<<"Can't convert grabbed result to PylonImage";
        return;
    }

    emit frameGrabbed(toQImage(pylonImage).convertToFormat(QImage::Format_RGB32));
}

QImage QPylonImageEventHandler::toQImage(const CPylonImage &pylonImage)
{
    int width = pylonImage.GetWidth();
    int height = pylonImage.GetHeight();
    const void *buffer = pylonImage.GetBuffer();
    int step = pylonImage.GetAllocatedBufferSize() / height;

    return QImage(static_cast<const uchar*>(buffer),
                  width, height, step,
                  QImage::Format_RGB888).copy();
}

QPylonCamera::QPylonCamera(QObject *parent) :
    QObject(parent),
    mImageEventHandler(new QPylonImageEventHandler(this)),
    mVideoSurface(Q_NULLPTR),
    mCamera(Q_NULLPTR),
    mRect()
{
    PylonInitialize();
    open();

    connect(qApp, &QGuiApplication::aboutToQuit, [=]() {
        stopGrabbing();

        mCamera->DeregisterImageEventHandler(mImageEventHandler);
        mCamera->Close();
        mCamera->DestroyDevice();
        delete mCamera;
        mCamera = Q_NULLPTR;

        PylonTerminate();
    });
}

QPylonCamera::~QPylonCamera()
{
}

void QPylonCamera::open()
{
    if(isOpen())
        return;

    try {
        // Create an instant camera object with the camera device found first.
        mCamera = new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());

        // Print the camera information.
        qInfo()<<"Using device : "<< mCamera->GetDeviceInfo().GetModelName();
        qInfo()<<"Friendly Name: "<< mCamera->GetDeviceInfo().GetFriendlyName();
        qInfo()<<"Full Name    : "<< mCamera->GetDeviceInfo().GetFullName();
        qInfo()<<"SerialNumber : "<< mCamera->GetDeviceInfo().GetSerialNumber();

        // Handle image event
        mCamera->RegisterImageEventHandler(mImageEventHandler, RegistrationMode_Append, Cleanup_Delete);

        mCamera->Open();

        // Get the integer nodes describing the AOI.
        const INodeMap& nodemap = mCamera->GetNodeMap();
        CIntegerPtr offsetX( nodemap.GetNode( "OffsetX"));
        CIntegerPtr offsetY( nodemap.GetNode( "OffsetY"));
        CIntegerPtr width(   nodemap.GetNode( "Width"));
        CIntegerPtr height(  nodemap.GetNode( "Height"));

        qInfo()<<"OffsetX: "<<offsetX->GetValue();
        qInfo()<<"OffsetY: "<<offsetY->GetValue();
        qInfo()<<"Width: "<<width->GetValue();
        qInfo()<<"Height: "<<height->GetValue();
        mRect = QRect(offsetX->GetValue(),
                      offsetY->GetValue(),
                      width->GetValue(),
                      height->GetValue());

    }  catch (GenICam::GenericException &e) {
        mCamera = Q_NULLPTR;
        qWarning() << "Camera Error: " << e.GetDescription();
    }
}

QAbstractVideoSurface *QPylonCamera::videoSurface() const
{
    return mVideoSurface;
}

void QPylonCamera::setVideoSurface(QAbstractVideoSurface *videoSurface)
{
    if(videoSurface == mVideoSurface)
        return;

    mVideoSurface = videoSurface;

    if(videoSurface != Q_NULLPTR)
        start();

    emit videoSurfaceChanged();
}

bool QPylonCamera::start()
{
    if(mVideoSurface != Q_NULLPTR) {
        QVideoFrame::PixelFormat f = QVideoFrame::pixelFormatFromImageFormat(QImage::Format_RGB32);
        QVideoSurfaceFormat format(mRect.size(), f);
        if(!mVideoSurface->start(format))
            qWarning()<<"VideoSurface can't start";
    } else {
        qWarning()<<"VideoSurface is null";
    }
    return startGrabbing();
}

void QPylonCamera::stop()
{
    stopGrabbing();
}

void QPylonCamera::capture()
{
    emit captured(mBuffer);
}

void QPylonCamera::renderFrame(const QImage &frame)
{
    mBuffer = frame;

    if (!mVideoSurface)
        return;

    if(!mVideoSurface->present(QVideoFrame(frame)))
        qWarning()<<"Can't render frame";
}

bool QPylonCamera::startGrabbing()
{
    if (!isOpen()) {
        qWarning()<<"Camera didn't open, can't start grabbing";
        return false;
    }

    connect(mImageEventHandler, &QPylonImageEventHandler::frameGrabbed,
            this, &QPylonCamera::renderFrame);

    mCamera->StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
    return true;
}

void QPylonCamera::stopGrabbing()
{
    if (!isOpen())
        return;

    disconnect(mImageEventHandler, &QPylonImageEventHandler::frameGrabbed,
               this, &QPylonCamera::renderFrame);

    if (mCamera->IsGrabbing())
        mCamera->StopGrabbing();
}
