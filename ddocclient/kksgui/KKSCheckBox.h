/***********************************************************************
 * Module:  KKSCheckBox.h
 * Author: yuriy 
 * Modified: 13 декабря 2008 г. 12:13:21
 * Purpose: Declaration of the class KKSCheckBox
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCheckBox_h)
#define __KKSSITOOOM_KKSCheckBox_h

#include <KKSAttrWidget.h>
#include <QString>
#include <QCheckBox>
#include <QVariant>

#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSCheckBox : public QCheckBox, public KKSAttrWidget
{
public:
    KKSCheckBox (const KKSAttrValue * attr, KKSIndAttrClass isSys, QWidget *parent=NULL);
    KKSCheckBox (const KKSAttrValue* attr, KKSIndAttrClass isSys, const QString& text, QWidget *parent=NULL);
    virtual ~KKSCheckBox (void);

signals:
    void valueChanged (qint64 id, KKSIndAttrClass sys, QVariant val);

public slots:
    void setState (int state);

private:
    //
    // Variables
    //

private:
    Q_OBJECT

};

#endif
