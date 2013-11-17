/***********************************************************************
 * Module:  KKSPixmap.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:26
 * Purpose: Declaration of the class KKSPixmap
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSPixmap_h)
#define __KKSSITOOOM_KKSPixmap_h

#include <QPixmap>
#include <QLabel>
#include <QVariant>
#include <KKSAttrWidget.h>

#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSPixmap : public QLabel, public KKSAttrWidget
{
    public:
        KKSPixmap (const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget *parent=NULL);
        KKSPixmap (const KKSAttrValue*attr, KKSIndAttrClass isSys, const QString& contents, QWidget *parent=NULL);
        virtual ~KKSPixmap (void);

        const QPixmap & pixmap() const;
        QByteArray save();

    signals:
        void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

    public slots:
        
        void setVal (const QString & newJpg);
        void openFile();
        void setPixmap(const QString & contents);
    
    private slots:
        void setPixmapPrivate();

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        int isSystem;
        QPixmap m_px;

    private:
        Q_OBJECT
};

#endif
