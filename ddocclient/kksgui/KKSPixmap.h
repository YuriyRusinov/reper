/***********************************************************************
 * Module:  KKSPixmap.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:13:26
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
        KKSPixmap (const KKSAttrValue*attr, bool isSys, QWidget *parent=NULL);
        KKSPixmap (const KKSAttrValue*attr, bool isSys, const QString& contents, QWidget *parent=NULL);
        virtual ~KKSPixmap (void);

        const QPixmap & pixmap() const;
        QByteArray save();

    signals:
        void valueChanged (int id, bool isSys, QVariant val);

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
//        bool isSystem;
        QPixmap m_px;

    private:
        Q_OBJECT
};

#endif
