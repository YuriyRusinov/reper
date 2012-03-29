/* 
 * File:   KKSVideoPlayer.h
 * Author: rusinov
 *
 * Created on 10 ќкт€брь 2011 г., 15:36
 */

#ifndef _KKSVIDEOPLAYER_H
#define	_KKSVIDEOPLAYER_H

#include <QLabel>
#include <QVariant>
#include "KKSAttrWidget.h"

#include "kksgui_config.h"

class QMovie;

class KKSAttrValue;

class _GUI_EXPORT KKSVideoPlayer : public QLabel, public KKSAttrWidget
{
public:
    KKSVideoPlayer (const KKSAttrValue*attr, bool isSys, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSVideoPlayer (void);

    QByteArray save (void);

signals:
    void valueChanged (int id, bool isSys, QVariant val);

public slots:

    void setVal (const QString & newMovie);
    void openFile (void);
    void setMovie (const QByteArray & contents);

private slots:
    void setMoviePrivate (void);

private:
    //
    // Variables
    //
//    const KKSIndAttr *attribute;
//    bool isSystem;
    QMovie * m_movie;

private:
    Q_OBJECT
};

#endif	/* _KKSVIDEOPLAYER_H */

