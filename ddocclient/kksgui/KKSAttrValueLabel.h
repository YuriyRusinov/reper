/***********************************************************************
 * Module:  KKSAttrValueLabel.h
 * Author:  sergey
 * Modified: 26 марта 2012 г. 12:13:26
 * Purpose: Declaration of the class KKSAttrValueLabel
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrValueLabel_h)
#define __KKSSITOOOM_KKSAttrValueLabel_h

#include <QLabel>

#include "kksgui_config.h"
#include <KKSList.h>
#include "KKSIndAttr.h"

class KKSAttrValue;
class KKSObject;

class _GUI_EXPORT KKSAttrValueLabel : public QLabel
{
public:
    KKSAttrValueLabel (KKSAttrValue * av, KKSIndAttr::KKSIndAttrClass isSystem, QWidget *parent=NULL);
    virtual ~KKSAttrValueLabel (void);

    void setAttrValue(KKSAttrValue * av);
    KKSAttrValue * attrValue() const;

signals:
    void clicked();
    void loadIOSrc (KKSObject **, QWidget *);
    void viewIOSrc(KKSObject *, QWidget *);
    void attrValueChanged();

    void loadHistory(const KKSAttrValue *, bool);
    void viewHistory(const KKSList<KKSAttrValue *> &);
    void viewDetailAttrVal (const KKSAttrValue *, int, int, QWidget *);
    void refreshDetailAttrVal (const KKSAttrValue *, int);

private slots:
    void viewAVal (const KKSAttrValue * av, int idAVal, QWidget * pWidget);
    void viewAHist (const KKSList<KKSAttrValue *> &);

public slots:
    void showAttrValueProps ();

protected:
    void mouseReleaseEvent ( QMouseEvent * event ) ;
    //void mouseMoveEvent( QMouseEvent * event ) ;
    void setLabelProps();

private:
    //
    // Variables
    //
    KKSAttrValue *m_av;
    KKSIndAttr::KKSIndAttrClass m_isSystem;

private:
    Q_OBJECT
};

#endif
