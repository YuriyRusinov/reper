#include <QImage>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "qimage_to_cvmat.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

    if (argc < 1)
    {
        cerr << "Set filename of test image";
        return 1;
    }
    QImage srcIm (argv[1]);
    QImage img2 = srcIm.convertToFormat(QImage::Format_ARGB32);
    Mat src_gray = QImageToCvMat (img2);
//    src_gray = imread(argv[1],0);
//    blur( src_gray, src_gray, Size(3,3) );
    Mat src_gray1 = src_gray.clone ();
    cvtColor (src_gray, src_gray1, CV_RGB2GRAY);

    Mat bwimg = src_gray1.clone();// > 127;
    vector<vector<Point> > contours;

    findContours( bwimg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE );
    unsigned int n = contours.size();
    cout << n << endl;

    for(unsigned int i=0;i<contours.size();i++)
    {
        approxPolyDP(Mat(contours[i]), contours[i], 10, true);

        if(i > 0)
        {
            cout << "Outer contour points \n";
        }
        else
            cout << "Inner contour points \n";

        //cout << contours[i].size();
        unsigned int nc = contours[i].size();
        cout << nc << endl;
        for(unsigned int j=0;j<contours[i].size();j++)
        {
            cout << "Point(x,y)=" << contours[i][j].x << "," << contours[i][j].y << endl;
            circle( src_gray, contours[i][j], 3, Scalar(0, 0, 255));//, FILLED, LINE_AA );
        }
        imshow( "Result", src_gray );
        waitKey(0);
    }

    return 0;
}

/*
cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData )
{
    switch ( inImage.format() )
    {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC4,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB32:
        case QImage::Format_RGB888:
        {
            if ( !inCloneImageData )
            {
               qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
            }

            QImage   swapped;

            if ( inImage.format() == QImage::Format_RGB32 )
               swapped = inImage.convertToFormat( QImage::Format_RGB888 );

            swapped = inImage.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(),
                            CV_8UC3,
                            const_cast<uchar*>(swapped.bits()),
                            static_cast<size_t>(swapped.bytesPerLine())
                            ).clone();
        }

        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        {
            qDebug () << __PRETTY_FUNCTION__ << "Indexed";
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC1,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}
*/
