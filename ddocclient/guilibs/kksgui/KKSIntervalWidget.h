#if !defined(__KKSSITOOOM_KKSIntervalWidget_h)
#define __KKSSITOOOM_KKSIntervalWidget_h

#include <QLineEdit>
#include <QComboBox>
#include <QVariant>
#include "kksgui_config.h"
#include <KKSAttrWidget.h>
#include "KKSAttrType.h"

class QHBoxLayout;

class KKSAttrValue;

class _GUI_EXPORT IntervalValue
{
public:
    IntervalValue(){amount = 0; iWhat = 0; what = "";}
    IntervalValue(int a, const QString & w)
    {
        setValue(a, w);
    }
    
    void setValue(int a, const QString & w)
    {
        amount = a; 
        what = w;
        if((iWhat = KKSAttrType::intervalNameToId(w)) == 0){
            amount = 0;
            what = "";
        }

        return;
    }

    void value(int * a, QString & w, int * iW) const
    {
        if(!a || !iW)
            return;

        *a = amount;
        w = what;
        *iW = iWhat;
    }

private:
    int amount;
    int iWhat;
    QString what;
};

class _GUI_EXPORT KKSIntervalWidget : public QWidget, public KKSAttrWidget
{
    public:
        KKSIntervalWidget (const KKSAttrValue *attr, KKSIndAttrClass isSys, QWidget *parent=NULL);
        ~KKSIntervalWidget (void);

        void setLineEdit (QLineEdit *le);
        void setComboUnits (QComboBox *cb);
        
        IntervalValue value();

    signals:
        void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

    public slots:
        void setValue (void);
        void setValue (const QString & str);
        void setValue(const IntervalValue & v);
        void setUnit (int index);

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        int isSystem;

        QLineEdit *lEValue;
        QComboBox *cbUnit;
        QHBoxLayout *hBLay;

    private:
        Q_OBJECT
};

#endif
