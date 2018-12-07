# Qt OpenCV Camera

When Qt camera backend not working, use OpenCV's camera to stream the video.

### This demo provides the basic functions of the camera model.

* `QtOpenCVCam::start`: start the camera stream.
* `QtOpenCVCam::stop`: stop the camera.
* `QtOpenCVCam::captured`: emit the captured frame.

Note: The demo only works with `QWidget` GUI, will fix `QML` later...
