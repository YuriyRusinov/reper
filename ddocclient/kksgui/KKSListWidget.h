/***********************************************************************
 * Module:  KKSListWidget.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:21
 * Purpose: Declaration of the class KKSComboBox
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSListWidget_h)
#define __KKSSITOOOM_KKSListWidget_h

#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>

#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSListWidget : public QListWidget, public KKSAttrWidget
{
public:
    KKSListWidget(const KKSAttrValue* attr, int isSys, QWidget *parent=NULL);
    virtual ~KKSListWidget(void);

    //void addItem(const QString & value, int itemData);
    //void setItemData(int index);

signals:
    void valueChanged (int id, int sys, QVariant val);

protected:

public slots:
    //void setCurrentData (int index);
    void changeValue(QListWidgetItem * it);


private:
//    const KKSIndAttr* attribute;
//    int isSystem;

private:
    Q_OBJECT
};

#endif
