#include <QImage>
#include <QtDebug>
#include "qimage_to_cvmat.h"

cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData )
{
/*    uchar* b = const_cast<uchar*> (inImage.bits ());
    int c = inImage.bytesPerLine();
    return cv::Mat(inImage.height(), inImage.width(), CV_8UC4, b, c).clone();
*/
    qDebug () << __PRETTY_FUNCTION__ << inImage.format ();
    switch ( inImage.format() )
    {
        //
        // 8-bit, 4 channel
        //
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

        //
        // 8-bit, 3 channel
        //
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

        //
        // 8-bit, 1 channel
        //
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
