/***********************************************************************
 * Module:  KKSDateTimeEdit.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:31
 * Purpose: Declaration of the class KKSDateTime
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSDateTimeEdit_h)
#define __KKSSITOOOM_KKSDateTime_h

#include <KKSAttrWidget.h>
#include <QDateTimeEdit>
#include <QVariant>
#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSDateTimeEdit : public QDateTimeEdit, public KKSAttrWidget
{
    public:
        KKSDateTimeEdit (const KKSAttrValue *attr, bool isSys, QWidget *parent=NULL);
        KKSDateTimeEdit (const KKSAttrValue *attr, bool isSys, const QDateTime& datetime, QWidget *parent=NULL);
        KKSDateTimeEdit (const KKSAttrValue *attr, bool isSys, const QDate& date, QWidget *parent=NULL);
        KKSDateTimeEdit (const KKSAttrValue *attr, bool isSys, const QTime& time, QWidget * parent=NULL);
        virtual ~KKSDateTimeEdit (void);

    signals:
        void valueChanged (int id, bool isSys, QVariant val);

    public slots:
        void setDateTime (const QDateTime& dt);

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        bool isSystem;

    private:
        Q_OBJECT
};

#endif
