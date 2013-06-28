/***********************************************************************
 * Module:  KKSComboBox.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:21
 * Purpose: Declaration of the class KKSComboBox
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSComboBox_h)
#define __KKSSITOOOM_KKSComboBox_h

#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QComboBox>
#include <QVariant>

#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSComboBox : public QComboBox, public KKSAttrWidget
{
    public:
        KKSComboBox (const KKSAttrValue * attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent=NULL);
        virtual ~KKSComboBox (void);
        
        //void addItem(const QString & value, int itemData);
        //void setItemData(int index);

    signals:
        void valueChanged (qint64 id, KKSIndAttr::KKSIndAttrClass sys, QVariant val);

    protected:

    public slots:
        void setCurrentData (int index);

    private:
        /* 
         * Для элемента типа "список значений из справочника" необходимо загрузить из БД записи из заданной таблицы и колонки. То что находится в колонке, должно отображаться в комбобоксе, но при этом должен храниться идентификатор каждой записи.
         */
        //int loadRecords(void);

    private:
//        const KKSCategoryAttr* attribute;

    private:
        Q_OBJECT
};

#endif
