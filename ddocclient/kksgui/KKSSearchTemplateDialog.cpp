/* 
 * File:   KKSSearchTemplateDialog.cpp
 * Author: rusinov
 * 
 * Created on 27 Март 2013 г., 11:41
 */
#include <QSplitter>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QtDebug>

#include <KKSSearchTemplate.h>
#include <KKSMap.h>
#include <KKSAttribute.h>
#include "kksfilterseditorform.h"
#include "savesearchtemplateform.h"
#include "KKSSearchTemplateDialog.h"

KKSSearchTemplateDialog::KKSSearchTemplateDialog(KKSSearchTemplate * st,  const KKSCategory * _c, const QString & tableName, bool forIO, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    searchTemplate (st),
    stForm (new SaveSearchTemplateForm (st, false)),
    filterForm (new KKSFiltersEditorForm (_c, tableName, KKSMap<int, KKSAttribute *>(), forIO, st, false)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    if (searchTemplate)
        searchTemplate->addRef ();
    
    init ();
}

KKSSearchTemplateDialog::~KKSSearchTemplateDialog()
{
    if (searchTemplate)
        searchTemplate->release ();
    delete stForm;
    delete filterForm;
}

KKSSearchTemplate * KKSSearchTemplateDialog::getSearchTemplate (void) const
{
    return searchTemplate;
}

void KKSSearchTemplateDialog::init (void)
{
    QGridLayout * gLay = new QGridLayout (this);
    QSplitter * sp = new QSplitter (Qt::Horizontal);
    sp->addWidget (stForm);
    sp->addWidget (filterForm);
    
    gLay->addWidget (sp, 0, 0, 1, 1);
    QHBoxLayout * hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch (1);
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);
}

SaveSearchTemplateForm * KKSSearchTemplateDialog::stypeWidget (void) const
{
    return stForm;
}

KKSFiltersEditorForm * KKSSearchTemplateDialog::sFilterWidget (void) const
{
    return filterForm;
}