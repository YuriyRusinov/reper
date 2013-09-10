#include <QMessageBox>
#include <QtDebug>
#include <QLayout>
#include <QGroupBox>
#include <QPalette>
#include <QColor>
#include <QSizePolicy>

#include "KKSAttrValue.h"
#include "KKSAttribute.h"
#include "KKSAttribute.h"
#include "KKSComplexAttrWidget.h"
#include "KKSAttrType.h"
//#include "KKSAttributesFactory.h"
#include "KKSObjEditor.h"

KKSComplexAttrWidget :: KKSComplexAttrWidget (const KKSAttrValue *attr, 
                                              KKSIndAttr::KKSIndAttrClass isSys, 
                                              //KKSAttributesFactory * a,
                                              KKSObjEditor * objEditor,
                                              QWidget *parent, 
                                              Qt::WindowFlags flags)
    : QWidget (parent ,flags), 
      KKSAttrWidget(attr, isSys),
      //m_awf(a),
      m_gAttrLay (new QGridLayout()),
      m_groupBox (new QGroupBox(this)),
      m_objEditor(objEditor)
{
    this->setLayout (m_gAttrLay);
    m_gAttrLay->setContentsMargins (0, 0, 0, 0);
    m_gAttrLay->setAlignment (Qt::AlignJustify | Qt::AlignVCenter);
/*
    this->setAutoFillBackground(true);
    QColor b(Qt::red);
    QColor w(Qt::green);
    QPalette p(b, w);
    this->setPalette(p);
*/
    //m_groupBox = new QGroupBox(this);
    m_groupBox->setTitle(m_av->attribute()->title() + tr(" (complex attribute)"));
    m_groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGridLayout * g = new QGridLayout();
    m_groupBox->setLayout(g);

    m_gAttrLay->addWidget(m_groupBox, 0, 0, 1, 1);

}


KKSComplexAttrWidget :: ~KKSComplexAttrWidget(void)
{
}


void KKSComplexAttrWidget :: init()
{
    if(!m_av)
        return;

    //if(!m_awf)
    //    return;

    if(m_av->attrsValues().count() == 0)
    {
        return;
    }

    KKSMap<int, KKSCategoryAttr*> attrs = m_av->attribute()->attrs();
    KKSList<KKSCategoryAttr*> sortedAttrs;

    //
    // составной атрибут Ќ≈ может содержать атрибуты следующих типов:
    //    atCheckList  -- ???
    //    atCheckListEx  -- они вообще в другой таблице визуализируютс€
    //    atComplex  --  пока вложенность избегаем
    //    atParent  -- не логично. родитель чего? јтрибут-то составной
    //    atRecordColor  -- ну просто не логично эти типы иметь
    //    atRecordColorRef
    //    atRecordTextColor
    //    atRecordTextColorRef
    //
    //    все они отфильтровываютс€ на триггерном уровне Ѕƒ
    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = attrs.constBegin(); pa != attrs.constEnd(); pa++){
        if(
            //pa.value()->type()->attrType() == KKSAttrType::atCheckList || 
            pa.value()->type()->attrType() == KKSAttrType::atCheckListEx ||
            pa.value()->type()->attrType() == KKSAttrType::atComplex ||
            pa.value()->type()->attrType() == KKSAttrType::atParent ||
            pa.value()->type()->attrType() == KKSAttrType::atRecordColor ||
            pa.value()->type()->attrType() == KKSAttrType::atRecordColorRef ||
            pa.value()->type()->attrType() == KKSAttrType::atRecordTextColor ||
            pa.value()->type()->attrType() == KKSAttrType::atRecordTextColorRef
          )
        {
            continue;
        }

        sortedAttrs.append (pa.value());
    }

    if(sortedAttrs.count() == 0)
        return;

    qSort (sortedAttrs.begin(), sortedAttrs.end(), compareCategoryAttrs);


    for (int ii=0; ii<sortedAttrs.count(); ii++)
    {
        KKSCategoryAttr * ca = sortedAttrs[ii];
        KKSAttrValue * av = NULL;
        
        KKSMap<qint64, KKSAttrValue*> aav_list = m_av->attrsValues();
        for (KKSMap<qint64, KKSAttrValue*>::const_iterator pa = aav_list.constBegin(); pa != aav_list.constEnd(); pa++)
        {
            KKSAttrValue * av1 = pa.value();
            if(av1->attribute()->id() == ca->id()){
                av = av1;
                break;
            }
        }

        if (av)
            av->addRef ();

        //gAttrLayout->setVerticalSpacing (10);
        emit putAttrAttrOnWidget (av, 
                                  m_objEditor, //KKSObjEditor -- в данном контексте имеем право указать NULL. ѕоле используетс€ только в расширенных св-вах атрибута (показател€), которых у данного атрибута нет 
                                  qobject_cast<QGridLayout *>(m_groupBox->layout()), 
                                  ii, 
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacAttrAttr,
#else
                                  KKSIndAttr::iacAttrAttr,
#endif
                                  av->attribute()->tableName(), //таблица, из которой дл€ ссылочных атрибутов будут загружатьс€ значени€
                                  -1);//(c ? c->id():-1));

        if(av)
            av->release ();
    }

    

}

void KKSComplexAttrWidget :: setVal (const QString & newText)
{
    QVariant v (newText);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

QGridLayout * KKSComplexAttrWidget :: getLay (void) const
{
    return qobject_cast<QGridLayout *>(m_groupBox->layout());
}