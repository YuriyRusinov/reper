/***********************************************************************
 * Module:  KKSComplexAttrWidget.h
 * Author:  ksa
 * Modified: 19.06.2013 ã. 9:41
 * Purpose: Declaration of the class KKSComplexAttrWidget
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSComplexAttrWidget_h)
#define __KKSSITOOOM_KKSComplexAttrWidget_h

#include <QWidget>
#include <QVariant>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"


class KKSAttrValue;
class QGridLayout;
class QGroupBox;
//class KKSAttributesFactory;
class KKSObjEditor;

class _GUI_EXPORT KKSComplexAttrWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSComplexAttrWidget (const KKSAttrValue*attr, 
                          KKSIndAttrClass isSys, 
                          //KKSAttributesFactory * a,
                          KKSObjEditor * objEditor=0,
                          QWidget *parent=NULL, 
                          Qt::WindowFlags flags=0);
    
    ~KKSComplexAttrWidget(void);

    QGridLayout * getLay (void) const;
public slots:
    void setVal (const QString & newText);
    void init();

private slots:

signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);
    void putAttrAttrOnWidget(KKSAttrValue*, KKSObjEditor*, QGridLayout*, int, KKSIndAttrClass, QString, int);

protected:

private:

    //
    // Variables
    //

    QGridLayout * m_gAttrLay;
    QGroupBox * m_groupBox;
    //KKSAttributesFactory * m_awf;
    KKSObjEditor * m_objEditor;

private:
    Q_OBJECT
};

#endif
