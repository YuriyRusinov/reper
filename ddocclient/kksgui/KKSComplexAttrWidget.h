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
class KKSAttributesFactory;
class KKSObjEditor;

class _GUI_EXPORT KKSComplexAttrWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSComplexAttrWidget (const KKSAttrValue*attr, 
                          KKSIndAttr::KKSIndAttrClass isSys, 
                          KKSAttributesFactory * a,
                          KKSObjEditor * objEditor,
                          QWidget *parent=NULL, 
                          Qt::WindowFlags flags=0);
    
    ~KKSComplexAttrWidget(void);

public slots:
    void setVal (const QString & newText);

private slots:

signals:
    void valueChanged (qint64 id, KKSIndAttr::KKSIndAttrClass isSys, QVariant val);

protected:

private:
    void init();
    //
    // Variables
    //

    QGridLayout * m_gAttrLay;
    QGroupBox * m_groupBox;
    KKSAttributesFactory * m_awf;
    KKSObjEditor * m_objEditor;

private:
    Q_OBJECT
};

#endif
