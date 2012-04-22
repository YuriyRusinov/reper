#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QMessageBox>
#include <QColor>
#include <QPalette>
#include <QBrush>
#include <QtDebug>

#include <KKSAttrValue.h>
#include <KKSCategoryAttr.h>
#include <KKSAttrType.h>
#include "KKSColorWidget.h"

KKSColorWidget :: KKSColorWidget (const KKSAttrValue *attr, int isSys, QColor rgb_col, QWidget *parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(attr, isSys),
    lSampleText (new QLabel (tr("Sample text"), this)),
    tbColor (new QToolButton (this))
{
    QHBoxLayout * hLay = new QHBoxLayout (this);
    hLay->addWidget (lSampleText);
    bool autofillbk (rgb_col.isValid());

    lSampleText->setAutoFillBackground (autofillbk);
    hLay->addWidget (tbColor);

    tbColor->setText (tr("..."));
    setColor (rgb_col);

    connect (tbColor, SIGNAL (clicked()), this, SLOT (setColor()) );
}

KKSColorWidget :: ~KKSColorWidget (void)
{
}

void KKSColorWidget :: setColor (void)
{
    QColor rgb_col = QColorDialog::getColor (Qt::white, this);
    if (!rgb_col.isValid()){
        int u = QMessageBox::question (this, 
                                       tr("Color choose"),
                                       tr("You press cancel on color choose dialog.\n"
                                          "Do you want to clear the color value and set system defaults or just save current value?\n"
                                          "Press YES to clear the color value or NO to remains current value"),
                                       QMessageBox::Yes | QMessageBox::No
                                      );
        if(u == QMessageBox::No)
            return;
    }
    //qDebug () << __PRETTY_FUNCTION__ << QColor (rgb_col);
    
    emit valueChanged (m_av->id(), m_isSystem, QVariant (rgb_col));
    setColor (rgb_col);
}

void KKSColorWidget :: setColor (QColor rgb_col)
{
    if (!rgb_col.isValid())
    {
        lSampleText->setAutoFillBackground (false);
        return;
    }

    if (m_av->attribute()->type()->attrType() == KKSAttrType::atRecordColor ||
        m_av->attribute()->type()->attrType() == KKSAttrType::atRecordColorRef)
    {
        QColor bkCol (rgb_col);
        QPalette pal (bkCol);//= lSampleText->palette ();
        QString text = lSampleText->text ();
        pal.setBrush (QPalette::Window, QBrush (bkCol));
        lSampleText->setPalette (pal);
        lSampleText->setAutoFillBackground (true);
    }
    else if (m_av->attribute()->type()->attrType() == KKSAttrType::atRecordTextColor ||
             m_av->attribute()->type()->attrType() == KKSAttrType::atRecordTextColorRef)
    {
        QColor fgCol (rgb_col);
        QPalette pal = lSampleText->palette ();
        pal.setColor (QPalette::WindowText, fgCol);
        lSampleText->setPalette (pal);
    }
}

void KKSColorWidget :: hideToolButton (void)
{
    tbColor->setVisible (false);
}

void KKSColorWidget :: showToolButton (void)
{
    tbColor->setVisible (true);
}
