#include <QGridLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QEventLoop>
#include <QPointer>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QMessageBox>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QModelIndex>
#include <QAction>
#include <QtDebug>

#include <KKSCategory.h>
#include <KKSCategoryAttr.h>
#include <KKSAttrType.h>
#include <KKSMap.h>
#include <KKSRubric.h>
#include <KKSType.h>
#include <KKSAccessEntity.h>

#include "KKSEventFilter.h"
#include "KKSAttributesEditor.h"
#include "ui/kkscatattreditor.h"
#include "KKSRecWidget.h"
#include "KKSCatEditor.h"
#include "kksincludeswidget.h"
#include "kksstuffform.h"

KKSCatEditor :: KKSCatEditor (KKSCategory *c, 
                              KKSRecWidget * rw, 
                              KKSRecWidget * rtw, 
                              KKSRecWidget * raw,
                              KKSRecWidget * rTemplatesW, 
                              KKSRecWidget * rTableTemplatesW, 
                              KKSRecWidget * rAttrTemplatesW, 
                              const KKSMap<int, KKSType *>& catTypesList, 
                              int idCatType0, 
                              bool mode, 
                              QWidget *parent, 
                              Qt::WindowFlags f)
    : KKSDialog (parent, f),
    pCategory (c),
    pTableCategory (c->tableCategory()),
    pRecAttrCategory (c->recAttrCategory()),
    catTypes (catTypesList),
    tableType (0),
    gCatLayout (new QGridLayout()),
    tabCat (new QTabWidget()),
    pbOk (new QPushButton (tr("&OK"))),
    pbCancel (new QPushButton (tr("&Cancel"))),
    pbApply (new QPushButton (tr("&Apply"))),
    lEID (new QLineEdit()),
    lEName (new QLineEdit()),
    cbChildCat (new QComboBox()),
    tbChildCat (new QToolButton ()),
    cbTypes (new QComboBox()),
    cbGlobal(0),
    recWidget (rw),
    recTableW (rtw),
    recAttrW (raw), 
    recCatTemplatesW (rTemplatesW),
    recTableCatTemplatesW (rTableTemplatesW),
    recAttrCatTemplatesW (rAttrTemplatesW),
    rubrW (0),
    sForm (0),
    isCloseIgnored (false)
{
    if (pCategory)
        pCategory->addRef ();

    if (pTableCategory)
        pTableCategory->addRef ();
    
    if (pRecAttrCategory)
        pRecAttrCategory->addRef ();

    this->setLayout (gCatLayout);
    int cInd = 2;
    for (KKSMap<int, KKSType*>::const_iterator pt = catTypes.constBegin(); pt != catTypes.constEnd(); pt++)
    {
        cbTypes->addItem (pt.value()->name(), pt.key());
        if (pt.key() == idCatType0)
            cInd = cbTypes->count()-1;
    }

    this->init_widgets ();
    this->init_templates ();

    if (recCatTemplatesW)
    {
        connect (recCatTemplatesW->actAdd, SIGNAL (triggered()), this, SLOT (addTemplate()) );
        connect (recCatTemplatesW->actEdit, SIGNAL (triggered()), this, SLOT (editTemplate()) );
        connect (recCatTemplatesW->actDel, SIGNAL (triggered()), this, SLOT (delTemplate()) );
        recCatTemplatesW->setEnabled (pCategory->id() >0);
    }

    if (recTableCatTemplatesW)
    {
        connect (recTableCatTemplatesW->actAdd, SIGNAL (triggered()), this, SLOT (addTableTemplate()) );
        connect (recTableCatTemplatesW->actEdit, SIGNAL (triggered()), this, SLOT (editTableTemplate()) );
        connect (recTableCatTemplatesW->actDel, SIGNAL (triggered()), this, SLOT (delTableTemplate()) );
        recTableCatTemplatesW->setEnabled (pCategory->id() >0);
    }
    
    if (recAttrCatTemplatesW)
    {
        connect (recAttrCatTemplatesW->actAdd, SIGNAL (triggered()), this, SLOT (addIndTemplate()) );
        connect (recAttrCatTemplatesW->actEdit, SIGNAL (triggered()), this, SLOT (editIndTemplate()) );
        connect (recAttrCatTemplatesW->actDel, SIGNAL (triggered()), this, SLOT (delIndTemplate()) );
    }

    if (recWidget)
    {
        connect (recWidget->actAdd, SIGNAL (triggered()), this, SLOT (addAttribute()) );
        connect (recWidget->actEdit, SIGNAL (triggered()), this, SLOT (editAttribute()) );
        connect (recWidget->actDel, SIGNAL (triggered()), this, SLOT (delAttribute()) );
        connect (recWidget, SIGNAL (entityDoubleClicked()), this, SLOT (editAttribute()) );
    }

    if (recTableW)
    {
        connect (recTableW->actAdd, SIGNAL (triggered()), this, SLOT (addTableAttribute()) );
        connect (recTableW->actEdit, SIGNAL (triggered()), this, SLOT (editTableAttribute()) );
        connect (recTableW->actDel, SIGNAL (triggered()), this, SLOT (delTableAttribute()) );
        connect (recTableW, SIGNAL (entityDoubleClicked()), this, SLOT (editTableAttribute()) );
    }

    if (recAttrW)
    {
        connect (recAttrW->actAdd, SIGNAL (triggered()), this, SLOT (addIndicator()) );
        connect (recAttrW->actEdit, SIGNAL (triggered()), this, SLOT (editIndicator()) );
        connect (recAttrW->actDel, SIGNAL (triggered()), this, SLOT (delIndicator()) );
        connect (recAttrW, SIGNAL (entityDoubleClicked()), this, SLOT (editIndicator()) );
    }

    bool isType ((c && c->id() > 0) || (c && c->type() && c->type()->id() == 10));
    cbTypes->setEnabled (!isType);
    int cTypeId = c && c->type() ? c->type()->id() : cbTypes->itemData (cbTypes->currentIndex(), Qt::UserRole).toInt ();
    QWidget * taw = this->recTableW->parentWidget();//this->tabCat->widget (4);
    if (taw)
        //taw->setVisible (cTypeId == 1 || cTypeId == 2 || cTypeId == 8 || cTypeId == 12); //справочник, журнал, системный справочник, шаблон поискового запроса
        taw->setVisible (cTypeId == 1 || cTypeId == 2 || cTypeId == 8); //справочник, журнал, системный справочник

    cbTypes->setCurrentIndex(-1);
    connect (cbTypes, SIGNAL (currentIndexChanged(int)), this, SLOT (setCurrentType(int)) );
    cbTypes->setCurrentIndex (cInd);
    connect (tbChildCat, SIGNAL (clicked()), this, SLOT (createChildCat()) );
    connect (lEName, SIGNAL (textEdited (const QString&)), this, SLOT (setTableCategoryName (const QString&)) );

    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    pbOk->setDefault (true);
    pbOk->setAutoDefault (true);
    QKeySequence cancel_key (Qt::Key_Escape);
    pbCancel->setShortcut (cancel_key);

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSCatEditor :: ~KKSCatEditor (void)
{
    if (pCategory)
        pCategory->release ();
    if (pTableCategory)
        pTableCategory->release ();
    if (pRecAttrCategory)
        pRecAttrCategory->release ();
    catTypes.clear ();
}

void KKSCatEditor :: save (void)
{
    this->apply ();
}

void KKSCatEditor :: accept (void)
{
    this->apply ();
    if (!isCloseIgnored)
        KKSDialog::accept ();
}

void KKSCatEditor :: apply (void)
{
    if (lEName->text().isEmpty())
    {
        QMessageBox::warning (this, tr ("Save category"), tr ("Please set category name"), QMessageBox::Ok);
        isCloseIgnored = true;
        return;
    }

    pCategory->setName (lEName->text());

    if (rubrW && rubrW->rootRubric ())
        pCategory->setRootRubric (rubrW->rootRubric ());
    if ( pCategory->tableCategory() && pTableCategory && !pTableCategory->attributes().contains(1) )
    {
        int resID = QMessageBox::question (this, tr ("Save child category"), tr ("Creation category for table without system attribute ID does not allow. Do you want to add it ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (resID != QMessageBox::Yes)
        {
            isCloseIgnored = true;
            return;
        }
        else
        {
            emit setAttribute (1, pTableCategory, recTableW->getSourceModel(), this);
            isCloseIgnored = false;
        }
    }
    else
        isCloseIgnored = false;

    pCategory->setAsGlobal(cbGlobal->isChecked());

    if (pTableCategory)
    {
        pCategory->setTableCategory(pTableCategory);//->setId (pTableCategory->id());
        emit saveCategory (pTableCategory, pTableCategory->id(), 10, this);
    }
    
    if (pRecAttrCategory)
    {
        pCategory->setRecAttrCategory(pRecAttrCategory);
        emit saveCategory (pRecAttrCategory, pRecAttrCategory->id(), 10, this);
    }

    //cbChildCat->itemData (cbChildCat->currentIndex()).toInt()
    emit saveCategory (pCategory, (pTableCategory ? pTableCategory->id() : -1), cbTypes->itemData (cbTypes->currentIndex()).toInt(), this);

    bool isSaved (pCategory->id() > 0 && !isCloseIgnored);
    recCatTemplatesW->setEnabled (isSaved);
    if (pCategory->tableCategory() && recTableCatTemplatesW)
        recTableCatTemplatesW->setEnabled (isSaved);
    cbTypes->setEnabled (!isSaved);
    isCloseIgnored = !isSaved;
}

void KKSCatEditor :: init_widgets (void)
{
    this->init_parameters ();
    this->init_attributes ();
    this->initTableAttributes ();
    this->initIndicators ();
}

void KKSCatEditor :: init_parameters (void)
{
    gCatLayout->addWidget (tabCat, 0, 0, 1, 1);
    QWidget *catParWidget = new QWidget (this);
    QGridLayout *gCatPLayout = new QGridLayout ();
    gCatPLayout->setVerticalSpacing (6);

    catParWidget->setLayout (gCatPLayout);
    tabCat->insertTab (0, catParWidget, tr("Parameters"));
    QSizePolicy lSp (QSizePolicy::Minimum, QSizePolicy::Fixed);
    QSizePolicy cbSp (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QHBoxLayout *hBoxIDLay = new QHBoxLayout ();
    QLabel *lID = new QLabel ("ID:");
    lID->setSizePolicy (lSp);
    gCatPLayout->addWidget (lID, 0, 0, 1, 1, Qt::AlignRight);
    lEID->setText (QString("%1").arg (pCategory->id() >= 0 ? pCategory->id() : 0));
    lEID->setReadOnly (true);
    hBoxIDLay->addWidget (lEID, 0, 0);
    gCatPLayout->addLayout (hBoxIDLay, 0, 1, 1, 1);

    QHBoxLayout *hBoxNameLay = new QHBoxLayout ();
    QLabel *lName = new QLabel (tr("Name:"));
    gCatPLayout->addWidget (lName, 1, 0, 1, 1, Qt::AlignRight);
    lEName->setText (pCategory->name());
    hBoxNameLay->addWidget (lEName);//, 1, 1, 1, 1);
    gCatPLayout->addLayout (hBoxNameLay, 1, 1, 1, 1);

    QHBoxLayout *hTypeLay = new QHBoxLayout ();
    QLabel *lType = new QLabel (tr("Type :"));
    lType->setSizePolicy (lSp);
    gCatPLayout->addWidget (lType, 2, 0, 1, 1, Qt::AlignRight);
    hTypeLay->addWidget (cbTypes);
    cbTypes->setSizePolicy (cbSp);
    cbTypes->setSizeAdjustPolicy (QComboBox::AdjustToMinimumContentsLength);
    //hTypeLay->addSpacing (16);
    gCatPLayout->addLayout (hTypeLay, 2, 1, 1, 1);

    QHBoxLayout *hGlobalLay = new QHBoxLayout ();
    cbGlobal = new QCheckBox(tr("Mark as global"));
    cbGlobal->setSizePolicy(lSp);
    cbGlobal->setChecked(pCategory->id() <= 0 ? true : pCategory->isGlobal());
    cbGlobal->setEnabled (false);
    hGlobalLay->addWidget (cbGlobal);
    gCatPLayout->addLayout (hGlobalLay, 3, 1, 1, 1);

    QSpacerItem * sPar = new QSpacerItem (20, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    gCatPLayout->addItem (sPar, 4, 0, 1, 2);

    QVBoxLayout * vModalButtonsLay = new QVBoxLayout ();
    vModalButtonsLay->addStretch ();
    vModalButtonsLay->addWidget (pbOk);
    vModalButtonsLay->addWidget (pbCancel);

    gCatLayout->addLayout (vModalButtonsLay, 0, 1, 1, 1);

    int l, t, r, b;
    gCatPLayout->getContentsMargins (&l, &t, &r, &b);
    gCatPLayout->setContentsMargins (l, t, r, 0);
    gCatPLayout->getContentsMargins (&l, &t, &r, &b);
}

void KKSCatEditor :: setChildCat (const QMap<int, QString> chItems)
{
    cbChildCat->addItem (QString(), -1);
    QMap<int, QString>::const_iterator pItems;
    int cInd = -1;
    for (pItems = chItems.constBegin(); pItems != chItems.constEnd(); pItems++)
    {
        cbChildCat->addItem (pItems.value (), pItems.key());
        if (pCategory->tableCategory() && pItems.key() == pCategory->tableCategory()->id())
            cInd = cbChildCat->count ()-1;
    }
    cbChildCat->setCurrentIndex (cInd);
}

void KKSCatEditor :: init_attributes (void)
{
    if (!recWidget)
        return;
    QWidget *catAttrWidget = new QWidget (this);
    QGridLayout *gAttrLay = new QGridLayout ();
    catAttrWidget->setLayout (gAttrLay);
    tabCat->insertTab (1, catAttrWidget, tr("Attributes"));
    gAttrLay->addWidget (recWidget, 0, 0, 1, 1);
}

void KKSCatEditor :: initTableAttributes (void)
{
    if (!recTableW)
        return;
    QWidget * catTableAttrWidget = new QWidget (this);
    recTableW->setParent (catTableAttrWidget);
    QGridLayout * gAttrTLay = new QGridLayout (catTableAttrWidget);
    tabCat->insertTab (2, catTableAttrWidget, tr ("Table Attributes"));
    gAttrTLay->addWidget (recTableW, 0, 0, 1, 1);
}

void KKSCatEditor :: initIndicators (void)
{
    if (!recAttrW)
        return;
    QWidget * indW = new QWidget (this);
    recAttrW->setParent (indW);
    QGridLayout * gLay = new QGridLayout (indW);
    tabCat->insertTab (3, indW, tr("Indicators"));
    gLay->addWidget (recAttrW, 0, 0, 1, 1);
}

void KKSCatEditor :: setTemplates (KKSRecWidget *rtw)
{
    if (recCatTemplatesW)
    {
        recCatTemplatesW->setParent (0);
        delete recCatTemplatesW;
    }
    recCatTemplatesW = rtw;
    this->init_templates ();
}

void KKSCatEditor :: init_templates (void)
{
    if (recCatTemplatesW)
    {
        QWidget *catTemplWidget = new QWidget (this);
        QGridLayout *gTemplLay = new QGridLayout ();
        catTemplWidget->setLayout (gTemplLay);
        tabCat->insertTab (4, catTemplWidget, tr ("Category templates"));
        gTemplLay->addWidget (recCatTemplatesW, 0, 0, 1, 1);
    }
    if (recTableCatTemplatesW)
    {
        QWidget * catTableTemplW = new QWidget (this);
        QGridLayout * gTableTemplLay = new QGridLayout (catTableTemplW);
        tabCat->insertTab (5, catTableTemplW, tr ("Table category templates"));
        gTableTemplLay->addWidget (recTableCatTemplatesW, 0, 0, 1, 1);
    }
    if (recAttrCatTemplatesW)
    {
        QWidget * catAttrTemplW = new QWidget (this);
        QGridLayout * gAttrTemplLay = new QGridLayout (catAttrTemplW);
        tabCat->insertTab (6, catAttrTemplW, tr ("Indicator templates"));
        gAttrTemplLay->addWidget (recAttrCatTemplatesW, 0, 0, 1, 1);
    }
    this->init_rubrics ();
}

void KKSCatEditor :: init_rubrics (void)
{
    if (!rubrW)
        rubrW = new KKSIncludesWidget (pCategory->rootRubric(), true, false, true);

    QWidget * catRubrW = new QWidget (this);
    rubrW->setParent (catRubrW);
    QGridLayout * gRubrLay = new QGridLayout (catRubrW);
    tabCat->insertTab (7, catRubrW, tr ("Category rubrics") );
    gRubrLay->addWidget (rubrW, 0, 0, 1, 1);
    QTreeView *tv = rubrW->tvRubr();
    KKSEventFilter *ef = new KKSEventFilter (rubrW);
    tv->viewport()->installEventFilter (ef);
}

void KKSCatEditor :: addAttribute (void)
{
    QAbstractItemModel * aModel (recWidget->getSourceModel ());
    KKSCategory * c(pCategory);
    emit addAttrsIntoCat (c, aModel, this);
}

void KKSCatEditor :: addTableAttribute (void)
{
    QAbstractItemModel * aModel (recTableW->getSourceModel ());
    KKSCategory * c (pTableCategory);
    emit addAttrsIntoCat (c, aModel, this);
}

void KKSCatEditor :: addIndicator (void)
{
    if(!pRecAttrCategory){
        KKSType * cTableT = KKSType::createType10(); //child type
        pRecAttrCategory = new KKSCategory (-1, QString(), cTableT);
        pRecAttrCategory->setName("Indicators of " + pCategory->name());
        KKSAccessEntity * acl = new KKSAccessEntity ();
        pRecAttrCategory->setAccessRules (acl);
        pCategory->setRecAttrCategory (pRecAttrCategory);
        if (acl)
            acl->release ();
    }

    QAbstractItemModel * aModel (recAttrW->getSourceModel ());
    KKSCategory * c (pRecAttrCategory);
    emit addAttrsIntoCat (c, aModel, this);
}

void KKSCatEditor :: editAttribute (void)
{
    int idAttr = recWidget->getID();
    if (idAttr<0)
        return;

    KKSCategoryAttr * cAttr = pCategory->attribute (idAttr);
    if (!cAttr)
        return;

    KKSCatAttrEditor *acEditor = new KKSCatAttrEditor (cAttr, attrTypes, false, this);

    if (acEditor->exec () == QDialog::Accepted)
        emit setAttribute (cAttr, pCategory, recWidget->getSourceModel(), this);

    if (acEditor)
    {
        acEditor->setParent (0);
        delete acEditor;
    }
}

void KKSCatEditor :: editTableAttribute (void)
{
    int idAttr = recTableW->getID();
    if (idAttr<0)
        return;

    KKSCategoryAttr * cAttr = pTableCategory->attribute (idAttr);
    if (!cAttr)
        return;

    KKSCatAttrEditor *acEditor = new KKSCatAttrEditor (cAttr, attrTypes, false, this);

    if (acEditor->exec () == QDialog::Accepted)
        emit setAttribute (cAttr, pTableCategory, recTableW->getSourceModel(), this);

    if (acEditor)
    {
        acEditor->setParent (0);
        delete acEditor;
    }
    
}

void KKSCatEditor :: editIndicator (void)
{
    int idAttr = recAttrW->getID();
    if (idAttr<0)
        return;

    KKSCategoryAttr * cAttr = pRecAttrCategory->attribute (idAttr);
    if (!cAttr)
        return;

    KKSCatAttrEditor *acEditor = new KKSCatAttrEditor (cAttr, attrTypes, false, this);

    if (acEditor->exec () == QDialog::Accepted)
        emit setAttribute (cAttr, pRecAttrCategory, recAttrW->getSourceModel(), this);

    if (acEditor)
    {
        acEditor->setParent (0);
        delete acEditor;
    }
}

void KKSCatEditor :: delAttribute (void)
{
    int idAttr = recWidget->getID ();
    if (idAttr >=0 && (QMessageBox::question (this, tr("Delete attribute from category"), tr("Do you really want to delete attribute %1 from category ?").arg (idAttr), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes))
        emit delAttrFromCategory (idAttr, pCategory, recWidget->getSourceModel(), this);
}

void KKSCatEditor :: delTableAttribute (void)
{
    int idAttr = recTableW->getID ();
    if (idAttr >=0 && (QMessageBox::question (this, tr("Delete attribute from category"), tr("Do you really want to delete attribute %1 from category ?").arg (idAttr), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes))
        emit delAttrFromCategory (idAttr, pTableCategory, recTableW->getSourceModel(), this);
}

void KKSCatEditor :: delIndicator (void)
{
    int idAttr = recAttrW->getID ();
    if (idAttr >=0 && (QMessageBox::question (this, 
                                              tr("Delete attribute from category"), 
                                              tr("Do you really want to delete attribute %1 from category ?").arg (idAttr), 
                                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes)
       )
    {
        emit delAttrFromCategory (idAttr, pRecAttrCategory, recAttrW->getSourceModel(), this);
    }
}

void KKSCatEditor :: setAttrTypes (const KKSMap<int, KKSAttrType*>& aTypes)
{
    attrTypes = aTypes;
}

void KKSCatEditor :: addTemplate (void)
{
    if (pCategory && pCategory->id () > 0)
        emit addNewCategoryTemplate (this, pCategory->id (), recCatTemplatesW->getSourceModel());
}

void KKSCatEditor :: addTableTemplate (void)
{
    if (pTableCategory && pTableCategory->id () > 0)
        emit addNewCategoryTemplate (this, pTableCategory->id (), recTableCatTemplatesW->getSourceModel());
}

void KKSCatEditor :: addIndTemplate (void)
{
    if (this->pRecAttrCategory && pRecAttrCategory->id() > 0)
        emit addNewCategoryTemplate (this, pRecAttrCategory->id(), recAttrCatTemplatesW->getSourceModel());
}

void KKSCatEditor :: editTemplate (void)
{
    if (!recCatTemplatesW || !recCatTemplatesW->getSelectionModel()->currentIndex().isValid())
        return;
    int idTemplate = recCatTemplatesW->getID ();
    emit editCategoryTemplate (this, idTemplate, recCatTemplatesW->getSourceModel(), recCatTemplatesW->getSourceIndex());
}

void KKSCatEditor :: editTableTemplate (void)
{
    if (!recTableCatTemplatesW || !recTableCatTemplatesW->getSelectionModel()->currentIndex().isValid())
        return;
    int idTemplate = recTableCatTemplatesW->getID ();
    emit editCategoryTemplate (this, idTemplate, recTableCatTemplatesW->getSourceModel(), recTableCatTemplatesW->getSourceIndex());
}

void KKSCatEditor :: editIndTemplate (void)
{
    if (!recAttrCatTemplatesW || !recAttrCatTemplatesW->getSelectionModel())
        return;
    int idTemplate = recAttrCatTemplatesW->getID ();
    emit editCategoryTemplate (this, idTemplate, recAttrCatTemplatesW->getSourceModel (), recAttrCatTemplatesW->getSourceIndex() );
}

void KKSCatEditor :: delTemplate (void)
{
    if (!recCatTemplatesW)
        return;
    int idTemplate = recCatTemplatesW->getID ();
    
    int yes = QMessageBox::question (this, 
                                     tr("Delete template from category"), 
                                     tr("Do you really want to delete selected template?"), 
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    
    if (idTemplate >= 0 && ( yes == QMessageBox::Yes))
        emit delCategoryTemplate (this, idTemplate, recCatTemplatesW->getSourceModel(), recCatTemplatesW->getSourceIndex());
}

void KKSCatEditor :: delTableTemplate (void)
{
    if (!recTableCatTemplatesW)
        return;
    
    int idTemplate = recTableCatTemplatesW->getID ();
    int yes = QMessageBox::question (this, 
                                     tr("Delete template from category"), 
                                     tr("Do you really want to delete selected template?"), 
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    
    if (idTemplate >= 0 && (yes == QMessageBox::Yes) )
        emit delCategoryTemplate (this, idTemplate, recTableCatTemplatesW->getSourceModel(), recTableCatTemplatesW->getSourceIndex());
}

void KKSCatEditor :: delIndTemplate (void)
{
    if (!recAttrCatTemplatesW)
        return;
    int idTemplate = recAttrCatTemplatesW->getID ();
    if (idTemplate >= 0 && (QMessageBox::question (this, tr("Delete template from category"), tr("Do you really want to delete template %1 ?").arg (idTemplate), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Yes) )
        emit delCategoryTemplate (this, idTemplate, recAttrCatTemplatesW->getSourceModel(), recTableCatTemplatesW->getSourceIndex());
}

void KKSCatEditor :: setIsChildCat (bool isChild)
{
    cbChildCat->setEnabled (isChild);
    tbChildCat->setEnabled (isChild);
    //cbGlobal->setEnabled (isChild);
}

void KKSCatEditor :: setCurrentType (int index)
{
    bool isChildAvail (index <= 1);
    setIsChildCat (isChildAvail);
    //cbGlobal->setEnabled (true);

    int cTypeId = cbTypes->itemData(index, Qt::UserRole).toInt ();
    KKSType * cType = this->catTypes.value (cTypeId);
    pCategory->setType (cType);
    //bool isRef (cTypeId == 1 || cTypeId == 2 || cTypeId == 8 || cTypeId == 12);//справочник, журнал, системный справочник, шаблон поискового запроса
    bool isRef (cTypeId == 1 || cTypeId == 2 || cTypeId == 8);//справочник, журнал, системный справочник
    QWidget * taw = this->recTableW->parentWidget();
    QWidget * ttw = recTableCatTemplatesW->parentWidget ();
    if (taw)
        taw->setEnabled (isRef);
    if (ttw)
        ttw->setEnabled (isRef);

    if (isRef)
    {
        //KKSType * cTableType = catTypes.value (10);
        if (!pTableCategory)
        {
            pTableCategory = new KKSCategory(-1, tr("Table of %1").arg(pCategory->name()), tableType);
            KKSAccessEntity * acl = new KKSAccessEntity (*pCategory->getAccessRules());
            pTableCategory->setAccessRules (acl);
        }
        if (pCategory->id() <= 0)
            pCategory->setTableCategory (pTableCategory);
        //KKSCategory * pTabCat = new KKSCategory ();
    }
    else
    {
        pCategory->setTableCategory (0);
        if (pCategory->id() > 0)
        {
            tabCat->removeTab (5);
            tabCat->removeTab (2);
        }
    }
}

void KKSCatEditor :: createChildCat (void)
{
    int ind = cbChildCat->currentIndex ();
    if (ind >= 0)
        emit editChildCat (this, cbChildCat->itemData (ind, Qt::UserRole).toInt(), true);
    else
        emit addChildCat (this, 10, true);
}

KKSCategory *KKSCatEditor :: category (void)
{
    return pCategory;
}

const KKSCategory * KKSCatEditor :: category (void) const
{
    return pCategory;
}

void KKSCatEditor :: appendChildCat (int idCat, QString catName)
{
    this->cbChildCat->addItem (catName, idCat);
    this->cbChildCat->setCurrentIndex (this->cbChildCat->count()-1);
}

void KKSCatEditor :: updateChildCat (int idCat, QString catName)
{
    int ind = this->cbChildCat->findData (idCat);
    if (ind >= 0)
        this->cbChildCat->setItemText (ind, catName);
    else
        appendChildCat (idCat, catName);
}

QAbstractItemModel *KKSCatEditor :: getTemplateModel (void)
{
    if (!recCatTemplatesW)
        return 0;
    else
        return recCatTemplatesW->getModel ();
}

QModelIndex KKSCatEditor :: getSelectedTemplateIndex (void) const
{
    if (!recCatTemplatesW)
        return QModelIndex ();
    else
        return recCatTemplatesW->getCurrentIndex ();
}

QTreeView * KKSCatEditor :: getTemplView (void) const
{
    if (!recCatTemplatesW)
        return 0;
    else
        return recCatTemplatesW->getView ();
}

KKSStuffForm * KKSCatEditor :: getAccessWidget (void) const
{
    return sForm;
}

void KKSCatEditor :: setAccessWidget (KKSStuffForm * _sForm)
{
    if (sForm)
    {
        sForm->setParent (0);
        int nc = this->tabCat->count ();
        tabCat->removeTab (nc-1);
        delete sForm;
    }

    sForm = _sForm;
    if (!sForm)
        return;
    sForm->setParent (this);
    tabCat->insertTab (7, sForm, tr ("Access Rules"));

    connect (sForm, SIGNAL (accessRulesChanged (KKSAccessEntity *)), this, SLOT (setAccessRules (KKSAccessEntity *)));
}

void KKSCatEditor :: setAccessRules (KKSAccessEntity * acl)
{
    if (pCategory)
    {
        pCategory->setAccessRules (acl);
        this->isCloseIgnored = true;
    }
    if (pTableCategory)
    {
        pTableCategory->setAccessRules (acl);
        this->isCloseIgnored = true;
    }
    if (pRecAttrCategory)
    {
        pRecAttrCategory->setAccessRules (acl);
        this->isCloseIgnored = true;
    }
}

void KKSCatEditor :: setTableCategoryName (const QString& tName)
{
    pCategory->setName (tName);

    QString tableCName = tr("Table of %1").arg (tName);
    if (this->pTableCategory)
        pTableCategory->setName (tableCName);
    QString indCName = tr("Indicators of %1").arg (tName);
    if (pRecAttrCategory)
        this->pRecAttrCategory->setName (indCName);
}

void KKSCatEditor :: setTableType (KKSType * tType)
{
    if (tableType)
        tableType->release ();

    tableType = tType;

    if (tableType)
        tableType->addRef ();
}

void KKSCatEditor :: copyAttributesFrom (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QAbstractItemModel * aModel = recTableW->getSourceModel ();
    emit copyAttrsFromAnotherCat (pTableCategory, aModel, this);
}

void KKSCatEditor :: catDbError (void)
{
    this->isCloseIgnored = true;
}