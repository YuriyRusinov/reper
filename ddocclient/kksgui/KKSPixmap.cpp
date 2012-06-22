/***********************************************************************
 * Module:  KKSPixmap.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:26
 * Purpose: Implementation of the class KKSPixmap
 ***********************************************************************/

#include "KKSPixmap.h"
#include "KKSAttrValue.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QBuffer>


KKSPixmap :: KKSPixmap (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent)
    : QLabel (parent), KKSAttrWidget(attr, isSys)
{

    //setPixmap("");

    //connect (this, SIGNAL (textChanged()), this, SLOT (setValPrivate()) );
}

KKSPixmap :: KKSPixmap (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, const QString& contents, QWidget *parent)
    : QLabel (parent), KKSAttrWidget(attr, isSys)
{
    
    //this->setPlainText(contents);
    
    setPixmap(contents);

    //закомментированный вариант не будет работать в MDI-приложении kkssito_app,
    //поскольку кнопка "сохранить" вынесена на тулбар. 
    //В этом случае editingFinished не происходит
    //вариант с textEdited работоспособен.
    //наверное возможны более "правильные" варианиты решения этой проблемы, но мы их оставим за кадром
    //connect (this, SIGNAL (editingFinished()), this, SLOT (setVal()) );
    connect (this, SIGNAL (textChanged()), this, SLOT (setValPrivate()) );
}


KKSPixmap :: ~KKSPixmap (void)
{
   // TODO : implement
}

void KKSPixmap :: setVal (const QString & newXPM)
{
    QVariant v (newXPM);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

//void KKSPixmap :: setValPrivate ()
//{
    //QString newText = this->toPlainText();
    //QVariant v (newText);
    //emit valueChanged (attribute->id(), isSystem, v);
//}

void KKSPixmap::setPixmap(const QString & contents)
{
    QByteArray ba = contents.toLocal8Bit();
    bool ok = m_px.loadFromData(ba, "XPM");
    if(!ok){
        qWarning() << "Cannot construct QPixmap from raw data! XPM = " << contents;
        return;
    }

    setPixmapPrivate();
}

const QPixmap & KKSPixmap::pixmap() const
{
    return m_px;
}

void KKSPixmap::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image File"), 
                                                    QDir::currentPath(),
                                                    tr("Images (*.png *.xpm *.jpg *.PNG *.XPM *.JPG)"));

    if (!fileName.isEmpty()) {
        QImage image(fileName);
        if (image.isNull()) {
            QMessageBox::information(this, tr("Error"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        m_px = QPixmap::fromImage(image);

        setPixmapPrivate();
    }

    else{
        return;
    }

    QVariant v;
    QByteArray ba = save();
    v = QVariant(ba);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSPixmap::setPixmapPrivate()
{
    int w = m_px.width();
    int h = m_px.height();

    this->QLabel::setPixmap(m_px);

    this->resize(w, h);
}

QByteArray KKSPixmap::save()
{
    QByteArray bytes;
    QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);
    m_px.save(&buffer, "XPM"); // writes pixmap into bytes in PNG format
    buffer.close();

    return bytes;
}
