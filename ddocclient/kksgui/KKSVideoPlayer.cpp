/* 
 * File:   KKSVideoPlayer.cpp
 * Author: rusinov
 * 
 * Created on 10 ќкт€брь 2011 г., 15:36
 */
#include <QMovie>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QIODevice>
#include <QBuffer>
#include <QImageWriter>
#include <QImageReader>
#include <QtDebug>

#include <KKSAttrValue.h>
#include "KKSVideoPlayer.h"

KKSVideoPlayer::KKSVideoPlayer(const KKSAttrValue *attr, int isSys, QWidget * parent, Qt::WindowFlags flags)
    : QLabel (parent, flags), KKSAttrWidget(attr, isSys),
      m_movie (0)
{
}

KKSVideoPlayer::~KKSVideoPlayer()
{
    if (m_movie)
    {
        delete m_movie;
        m_movie = 0;
    }
}

void KKSVideoPlayer :: setVal (const QString & newMovie)
{
    if (!m_movie)
        return;

    QByteArray ba (newMovie.toLocal8Bit());
    QBuffer * buff = new QBuffer ();
    buff->setData (ba);
    m_movie->setDevice (buff);
}

void KKSVideoPlayer::openFile (void)
{
    QList<QByteArray> fmts = QMovie::supportedFormats ();
    QList<QByteArray> imFmts = QImageReader::supportedImageFormats ();
    qDebug () << __PRETTY_FUNCTION__ << fmts << imFmts;
    QString filter (tr("Images ("));
    for (int i=0; i<fmts.count(); i++)
        filter += QString("*.%1 ").arg (QString(fmts[i]));
    for (int i=fmts.count(); i<fmts.count()+imFmts.count(); i++)
    {
        filter += QString("*.%1").arg (QString(imFmts[i-fmts.count()]));
        if (i<fmts.count()+imFmts.count()-1)
            filter += QString (" ");
        else
            filter += QString(");;All files (*.*)");
    }
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Video File"),
                                                    QDir::currentPath(),
                                                    filter);

    if (!fileName.isEmpty())
    {
        if (m_movie)
        {
            delete m_movie;
            m_movie = 0;
        }
        QFile* imFile = new QFile(fileName);
        imFile->open (QIODevice::ReadOnly);
        m_movie = new QMovie (imFile);//Name);
        m_movie->setFileName (fileName);
        if (!m_movie) {
            QMessageBox::information(this, tr("Error"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        setMoviePrivate();
    }

    else{
        return;
    }

    QVariant v;
    QByteArray ba = save();
    v = QVariant(ba);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSVideoPlayer::setMovie (const QByteArray & contents)
{
    QBuffer * buff = new QBuffer ();//&contents);
    //qDebug () << __PRETTY_FUNCTION__ << contents;
    buff->setData (contents);
    buff->open (QIODevice::ReadOnly);
    QByteArray imFormat = QImageReader::imageFormat (buff);
    qDebug () << __PRETTY_FUNCTION__ << imFormat;
    if (!m_movie)
        m_movie = new QMovie (buff, "png");
    else
    {
        m_movie->setDevice (buff);
        //m_movie->setFormat ("XPM");
    }
    bool isOk = m_movie->isValid();
    if (!isOk)
        return;
    setMoviePrivate ();
}

void KKSVideoPlayer::setMoviePrivate (void)
{
    this->QLabel::setMovie (m_movie);

    m_movie->start ();
}

QByteArray KKSVideoPlayer::save (void)
{
    QByteArray ba;
    if (!m_movie)
        return ba;
    QBuffer * buff = new QBuffer (&ba);
    buff->open (QIODevice::WriteOnly);
    QImageWriter imWriter (buff, QByteArray ("png"));
    qDebug () << __PRETTY_FUNCTION__ << QImageWriter::supportedImageFormats ();
    bool isOk = true;
    for (int i=0; i<m_movie->frameCount() && isOk; i++)
    {
        m_movie->jumpToFrame (i);
        isOk = imWriter.write (m_movie->currentImage());
        if (!isOk)
            qDebug () << __PRETTY_FUNCTION__ << imWriter.errorString();
    }
    buff->close ();
    delete buff;
    return ba;
}