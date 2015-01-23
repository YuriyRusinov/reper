#ifndef _ImageWidget_H
#define _ImageWidget_H

#include <QWidget>
#include <QImage>

class QLabel;
class QToolButton;

class ImageWidget : public QWidget
{
public:
    ImageWidget (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~ImageWidget (void);

    void setImage (const QImage& im);

private slots:
    void loadImageFile (void);
    void selectRect (void);
    void saveImageToDb (void);
    void loadImageFromDb (void);

signals:
    void saveImage (const QImage& );
    void loadImage (void);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    QLabel * lRImage;
    QToolButton * tbLoadImage;
    QToolButton * tbSelect;
    QToolButton * tbSaveToDb;
    QToolButton * tbLoadFromDb;

    QImage rGIm;
private:
    Q_OBJECT
};
#endif
