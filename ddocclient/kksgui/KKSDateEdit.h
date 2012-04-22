/***********************************************************************
 * Module:  KKSDateEdit.h
 * Author: yuriy 
 * Modified: 13 декабря 2008 г. 12:13:21
 * Purpose: Declaration of the class KKSDateEdit
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSDateEdit_h)
#define __KKSSITOOOM_KKSDateEdit_h

#include <KKSAttrWidget.h>
#include <QString>
#include <QDateEdit>
#include <QDate>
#include <QVariant>

#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSDateEdit : public QDateEdit, public KKSAttrWidget
{
    public:
        KKSDateEdit (const KKSAttrValue *attr, int isSys, QWidget *parent=NULL);
        KKSDateEdit (const KKSAttrValue *attr, int isSys, const QDate& date, QWidget *parent=NULL);
        virtual ~KKSDateEdit (void);

    signals:
        void valueChanged (int id, int isSys, QVariant val);

    public slots:
        void setDate (const QDate& d);

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        int isSystem;

    private:
        Q_OBJECT
};

#endif
