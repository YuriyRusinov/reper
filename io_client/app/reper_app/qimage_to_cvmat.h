#ifndef _QImageToCVMat_H
#define _QImageToCVMat_H

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

class QImage;
using namespace cv;

cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData =true);

#endif
