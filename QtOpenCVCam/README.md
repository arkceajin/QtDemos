# Qt OpenCV Camera

When Qt camera backend not working, could use OpenCV's camera to stream the video.
OpenCV `cv::VideoCapture` as backend and wrapped in class `QtOpenCVCam` in file `qtopencvcam.h` & `qtopencvcam.cpp`. Could switch between QWidget and QML by MACRO, to see how to use in the both situations.

### `QtOpenCVCam` provides the basic functions of the camera model.

* `QtOpenCVCam::start`: start the camera stream.
* `QtOpenCVCam::stop`: stop the camera.
* `QtOpenCVCam::captured`: emit the captured frame.
