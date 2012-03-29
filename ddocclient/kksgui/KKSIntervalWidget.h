#if !defined(__KKSSITOOOM_KKSIntervalWidget_h)
#define __KKSSITOOOM_KKSIntervalWidget_h

#include <QLineEdit>
#include <QComboBox>
#include <QVariant>
#include "kksgui_config.h"
#include <KKSAttrWidget.h>

class QHBoxLayout;

class KKSAttrValue;

class _GUI_EXPORT KKSIntervalWidget : public QWidget, public KKSAttrWidget
{
    public:
        KKSIntervalWidget (const KKSAttrValue *attr, bool isSys, QWidget *parent=NULL);
        ~KKSIntervalWidget (void);

        void setLineEdit (QLineEdit *le);
        void setComboUnits (QComboBox *cb);

    signals:
        void valueChanged (int id, bool isSys, QVariant val);

    public slots:
        void setValue (void);
        void setUnit (int index);

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        bool isSystem;

        QLineEdit *lEValue;
        QComboBox *cbUnit;
        QHBoxLayout *hBLay;

    private:
        Q_OBJECT
};

#endif
