#include "kkssearchtemplateform.h"
#include "ui_kkssearch_template_form.h"

#include <KKSAttribute.h>
#include <KKSCategory.h>
#include "KKSCheckableModel.h"
#include "KKSEventFilter.h"
#include "KKSImage.h"
#include "KKSSearchTemplate.h"
#include "KKSEventFilter.h"
#include "KKSItemDelegate.h"
#include "defines.h"
#include <KKSFilter.h>

#include <QMessageBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStackedLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QTextEdit>
#include <QSizePolicy>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QListView>
#include <QFileDialog>
#include <QPaintDevice>
#include <QPainter>
#include <QPointF>
#include <QBuffer>
#include <QInputDialog>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QSortFilterProxyModel>
#include <QKeySequence>
#include <QAction>
#include <QtDebug>

KKSSearchTemplateForm :: KKSSearchTemplateForm (const KKSCategory * _c, 
                                              const QString & tableName,
                                              KKSMap<int, KKSAttribute *> attrsIO,
                                              bool forIO,
                                              KKSSearchTemplate * st,
                                              bool mode,
                                              QWidget *parent,
                                              Qt::WFlags f)
    : QDialog (parent, f),
    ui (new Ui::kkssearch_template_form),
    sortRefModel (new QSortFilterProxyModel (this)),
    c (_c),
    m_attrsIO (attrsIO),
    m_bForIO (forIO),
    sTempl (st),
    delSearchEntity (new QAction (this)),
    isDbSaved (false),
    m_parentTable(tableName)
{
    ui->setupUi(this);
    if (c)
        c->addRef();
    QKeySequence dKey (Qt::Key_Delete);//QKeySequence::Delete);
    delSearchEntity->setShortcut (dKey);
    addAction (delSearchEntity);

    init ();
    if (sTempl)
    {
        if (mode)
            sTempl->addRef ();
        KKSList<const KKSFilterGroup*> groups;
        groups.append (sTempl->getMainGroup());
        setFilters (groups);
    }

    ui->pbSaveToDb->setVisible (m_bForIO);
    ui->pbLoadFromDb->setVisible (m_bForIO);
    
    ui->pbOK->setVisible (mode);
    ui->pbCancel->setVisible (mode);
    connect (ui->pbOK, SIGNAL (clicked()), this, SLOT (saveSQLAccept()) );
    connect (ui->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (ui->pbAddGroup, SIGNAL (clicked()), this, SLOT (addGroup()) );
    connect (ui->pbShowSQL, SIGNAL (clicked()), this, SLOT (viewSQL()));
    connect (cbAttribute, SIGNAL (currentIndexChanged(int)), this, SLOT (attrChanged (int)) );
    connect (cbOper, SIGNAL (currentIndexChanged(int)), this, SLOT (setValueWidget(int)) );
    connect (pbAddFilter, SIGNAL (clicked()), this, SLOT (addFilter()) );
    connect (ui->pbSaveToDb, SIGNAL (clicked()), this, SLOT (saveSQLQuery()) );
    connect (ui->pbLoadFromDb, SIGNAL (clicked()), this, SLOT (loadSQLQuery()) );
    connect (delSearchEntity, SIGNAL (triggered()), this, SLOT (delFilter()) );
}

KKSSearchTemplateForm :: ~KKSSearchTemplateForm (void)
{
    if (c)
        c->release ();
    if (sTempl && ui->pbOK->isVisible())
        sTempl->release ();
}

void KKSSearchTemplateForm :: init()
{
//    twFilters = new QTreeView (ui->frame);
    KKSEventFilter *ef = new KKSEventFilter (this);
    ui->twFilters->viewport()->installEventFilter (ef);
    QGridLayout * layFilter = new QGridLayout();
    layFilter->setContentsMargins (0, 0, 0, 0);
    ui->gbFilter->setLayout (layFilter);

    lAttribute = new QLabel (tr ("Attribute :"), ui->gbFilter);
    layFilter->addWidget (lAttribute, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignVCenter); 

    lOper = new QLabel (tr ("Operation :"), ui->gbFilter);
    layFilter->addWidget (lOper, 0, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    lValue = new QLabel (tr ("Value :"), ui->gbFilter);
    layFilter->addWidget (lValue, 0, 2, 1, 2, Qt::AlignLeft | Qt::AlignVCenter);

    cbAttribute = new QComboBox (ui->gbFilter);
    cbAttribute->setSizeAdjustPolicy (QComboBox::AdjustToContents);
    cbAttribute->setCurrentIndex (-1);
    layFilter->addWidget (cbAttribute, 1, 0, 3, 1, Qt::AlignJustify | Qt::AlignTop);

    cbOper = new QComboBox (ui->gbFilter);
    layFilter->addWidget (cbOper, 1, 1, 3, 1, Qt::AlignLeft | Qt::AlignTop);

    wValue = new QWidget (ui->gbFilter);
    stLayValue = new QStackedLayout (wValue);
    stLayValue->setAlignment (Qt::AlignHCenter | Qt::AlignTop);
    stLayValue->setContentsMargins (0, 0, 0, 0);
    layFilter->addWidget (wValue, 1, 2, 1, 1, Qt::AlignLeft | Qt::AlignTop);

    pbAddFilter = new QPushButton (tr ("Add &filter"), ui->gbFilter);
    layFilter->addWidget (pbAddFilter, 1, 3, 3, 1, Qt::AlignLeft | Qt::AlignTop);

    QStandardItemModel * model = new QStandardItemModel(0, 4);
    model->setHeaderData(0, Qt::Horizontal, tr("Attribute"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Operation"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Value"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("Group operation"), Qt::DisplayRole);

    ui->twFilters->setModel (model);
    ui->twFilters->header()->resizeSection(0, 250);
    ui->twFilters->header()->resizeSection(1, 70);

    KKSItemDelegate *itemDeleg = new KKSItemDelegate (this);
    ui->twFilters->setItemDelegate (itemDeleg);
    KKSEventFilter *eFilter = new KKSEventFilter (this);
    ui->twFilters->viewport ()->installEventFilter (eFilter);

    //
    // скрыть SQL
    //
    ui->teSQLView->setVisible(false);
    ui->pbShowSQL->setText(tr("Show SQL"));
    initValuesWidgets ();

    initAttrs ();
    this->attrChanged (0);
    if (sTempl)
        ui->lESearchTemplateName->setText (sTempl->name());
//    initFilterTypes();
}

void KKSSearchTemplateForm :: setSearchTemplateModel (QAbstractItemModel * sTypeMod)
{
    if (!sTypeMod)
        return;
    QAbstractItemModel * oldModel = ui->tvSearchTemplateTypes->model();
    ui->tvSearchTemplateTypes->setModel (sTypeMod);
    if (oldModel && oldModel != sTypeMod)
        delete oldModel;
}

void KKSSearchTemplateForm :: selectTypeInd (const QModelIndex& wIndex)
{
    QItemSelectionModel * selMod = ui->tvSearchTemplateTypes->selectionModel();
    selMod->select(wIndex, QItemSelectionModel::ClearAndSelect);
    selMod->setCurrentIndex(wIndex, QItemSelectionModel::ClearAndSelect);
}

void KKSSearchTemplateForm :: initFilterTypes (KKSAttrType::KKSAttrTypes type)
{
    QString oper;
    QVariant data;

    cbOper->clear ();

    if (type == KKSAttrType::atUndef)
        return;

    oper = tr("Equal");
    data = QVariant((int)KKSFilter::foEq);
    cbOper->addItem(oper, data);
    cbOper->setItemData(0, "=", Qt::UserRole+1);

    oper = tr("Not equal");;
    data = QVariant((int)KKSFilter::foNotEq);
    cbOper->addItem(oper, data);
    cbOper->setItemData(1, "!=", Qt::UserRole+1);

    if (type == KKSAttrType::atDate ||
        type == KKSAttrType::atDateTime ||
        type == KKSAttrType::atDouble ||
        type == KKSAttrType::atInt ||
        type == KKSAttrType::atString ||
        type == KKSAttrType::atInterval ||
        type == KKSAttrType::atIntervalH ||
        type == KKSAttrType::atText ||
        type == KKSAttrType::atFixString)
    {
        oper = tr("Greather");;
        data = QVariant((int)KKSFilter::foGr);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, ">", Qt::UserRole+1);

        oper = tr("Not fewer");;
        data = QVariant((int)KKSFilter::foGrEq);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, ">=", Qt::UserRole+1);

        oper = tr("Fewer");;
        data = QVariant((int)KKSFilter::foLess);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, "<", Qt::UserRole+1);

        oper = tr("Not greather");;
        data = QVariant((int)KKSFilter::foLessEq);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, "<=", Qt::UserRole+1);
    }

    if (type == KKSAttrType::atString || 
        type == KKSAttrType::atText || 
        type == KKSAttrType::atFixString ||
        type == KKSAttrType::atXMLDoc)
    {
        oper = tr("Contain (LIKE '%AAA%')");
        data = QVariant((int)KKSFilter::foLikeIn);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, "like", Qt::UserRole+1);
        oper = tr("Start with (LIKE 'AAA%')");
        data = QVariant((int)KKSFilter::foLikeStart);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, "like", Qt::UserRole+1);
        oper = tr("End with (LIKE '%AAA')");
        data = QVariant((int)KKSFilter::foLikeEnd);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, "like", Qt::UserRole+1);
        oper = tr("Correspond to template (LIKE 'AAA')");
        data = QVariant((int)KKSFilter::foLike);
        cbOper->addItem(oper, data);
        cbOper->setItemData(cbOper->count()-1, "like", Qt::UserRole+1);
    }

    if (type == KKSAttrType::atCheckList ||
        type == KKSAttrType::atCheckListEx)
    {
        oper = tr("Involve");//tr ("Contains");
        data = QVariant ((int)KKSFilter::foIn);
        cbOper->addItem (oper, data);
        cbOper->setItemData(cbOper->count()-1, "in", Qt::UserRole+1);

        oper = tr("Not involve");//tr ("Not Contains");
        data = QVariant ((int)KKSFilter::foNotIn);
        cbOper->addItem (oper, data);
        cbOper->setItemData(cbOper->count()-1, "not in", Qt::UserRole+1);
    }

    oper = tr("Is null (NULL)");
    data = QVariant((int)KKSFilter::foIsNull);
    cbOper->addItem(oper, data);
    cbOper->setItemData(cbOper->count()-1, "is null", Qt::UserRole+1);
    
    oper = tr("Is not null (NOT NULL)");
    data = QVariant((int)KKSFilter::foIsNotNull);
    cbOper->addItem(oper, data);
    cbOper->setItemData(cbOper->count()-1, "is not null", Qt::UserRole+1);

    cbOper->setCurrentIndex (0);
}

void KKSSearchTemplateForm :: initAttrs()
{
    if (!c && m_attrsIO.isEmpty())
        return;

    m_attrsAll.clear ();
    QString text;
    QVariant data;

    KKSMap<int, KKSCategoryAttr *> attrs = c->attributes();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = attrs.constBegin(); pca != attrs.constEnd(); pca++)
    {
        KKSAttribute * attr = new KKSAttribute (*pca.value());
        m_attrsAll.insert (pca.key(), attr);
        attr->release ();
    }

    KKSMap<int, KKSAttribute *>::const_iterator pa;
    for (pa = m_attrsIO.constBegin(); pa != m_attrsIO.constEnd(); pa++)
    {
        if (!m_attrsAll.contains (pa.key()))
        {
            KKSAttribute * attr = new KKSAttribute (*pa.value());
            m_attrsAll.insert (pa.key(), attr);
            attr->release ();
        }
    }

    cbAttribute->addItem (tr("Search by all attributes"), -1);
    for (pa = m_attrsAll.constBegin(); pa != m_attrsAll.constEnd(); pa++)
    {
        KKSAttribute * attr = pa.value();
        text = attr->title();
        data = QVariant(attr->id());
        cbAttribute->addItem(text, data);
    }

    cbAttribute->setCurrentIndex (0);
}

void KKSSearchTemplateForm :: initValuesWidgets (void)
{
    QWidget * cWNum = new QWidget (wValue);
    //
    // Numeric attributes (int, double)
    //
    QVBoxLayout * vLay1 = new QVBoxLayout (cWNum);
    vLay1->setContentsMargins (0, 0, 0, 0);
    lEValue = new QLineEdit (cWNum);
    lEValue->setAlignment (Qt::AlignLeft | Qt::AlignTop);
    vLay1->addWidget (lEValue);
    vLay1->addStretch ();
    stLayValue->addWidget (cWNum);//insertWidget (0, cWNum);

    //
    // String or String with fixed length attributes
    //
    QWidget * cWStr = new QWidget (wValue);
    QVBoxLayout * vLay2 = new QVBoxLayout (cWStr);
    vLay2->setContentsMargins (0, 0, 0, 0);
    lEStrValue = new QLineEdit (cWStr);
    lEStrValue->setAlignment (Qt::AlignLeft | Qt::AlignTop);
    vLay2->addWidget (lEStrValue);
    chCaseSensitive = new QCheckBox (tr ("Case sensitive"), cWStr);
    //chCaseSensitive->setCheckState (Qt::Checked);
    chCaseSensitive->setEnabled (true);
    vLay2->addWidget (chCaseSensitive);
    vLay2->addStretch ();
    stLayValue->insertWidget (1, cWStr);

    //
    // List or parent attributes
    //
    QWidget * cWRef = new QWidget (wValue);
/*    QVBoxLayout * vLay3 = new QVBoxLayout (cWRef);
    vLay3->setContentsMargins (0, 0, 0, 0);
    cbValue = new QComboBox (cWRef);
    vLay3->addWidget (cbValue);
    vLay3->addStretch ();*/
    stLayValue->addWidget (cWRef);//insertWidget (2, cWRef);

    //
    // Text attributes
    //
    QWidget * cWText = new QWidget (wValue);
    QSizePolicy stp (QSizePolicy::Minimum, QSizePolicy::Preferred);
    cWText->setSizePolicy (stp);
    QVBoxLayout * vLay4 = new QVBoxLayout (cWText);
    vLay4->setContentsMargins (0, 0, 0, 0);
    //vLay4->setSizeConstraint (QLayout::SetMinimumSize);
    teValue = new QTextEdit (cWText);
    QSizePolicy sTextP (QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    teValue->setSizePolicy (sTextP);
    cWText->setSizePolicy (stp);
    teValue->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    chTextCaseSensitive = new QCheckBox (tr ("Case sensitive"), cWText);
    //chTextCaseSensitive->setSizePolicy (stp);
    //chTextCaseSensitive->setCheckState (Qt::Checked);
    chTextCaseSensitive->setEnabled (true);
    vLay4->addWidget (teValue);
    vLay4->addWidget (chTextCaseSensitive);//, 0, Qt::AlignTop);
    //vLay4->addStretch ();
    stLayValue->addWidget (cWText);
//    qDebug () << __PRETTY_FUNCTION__ << teValue->sizeHint () << teValue->minimumSizeHint ();

    //
    // bool attributes
    //
    QWidget * cWBool = new QWidget (wValue);
    QVBoxLayout * vLay5 = new QVBoxLayout (cWBool);
    vLay5->setContentsMargins (0, 0, 0, 0);
    chValue = new QCheckBox (tr ("is not set"), cWBool);
    connect (chValue, SIGNAL (stateChanged(int)), this, SLOT (setBoolValChanged(int)) );
    vLay5->addWidget (chValue);
    vLay5->addStretch ();
    stLayValue->addWidget (cWBool);//insertWidget (4, cWBool);

    //
    // date attributes
    //
    QWidget * cWDate = new QWidget (wValue);
    QVBoxLayout * vLay6 = new QVBoxLayout (cWDate);
    vLay6->setContentsMargins (0, 0, 0, 0);
    dValue = new QDateEdit (QDate::currentDate (), cWDate);
    vLay6->addWidget (dValue);
    vLay6->addStretch ();
    stLayValue->addWidget (cWDate);//insertWidget (5, cWDate);

    //
    // date-time attributes
    //
    QWidget * cWDateTime = new QWidget (wValue);
    QVBoxLayout * vLay7 = new QVBoxLayout (cWDateTime);
    vLay7->setContentsMargins (0, 0, 0, 0);
    dtValue = new QDateTimeEdit (QDateTime::currentDateTime(), cWDateTime);
    vLay7->addWidget (dtValue);
    vLay7->addStretch ();
    stLayValue->addWidget (cWDateTime);//insertWidget (6, cWDateTime);

    //
    // interval attributes
    //
    QWidget * cWInterval = new QWidget (wValue);
    QVBoxLayout * vLay8 = new QVBoxLayout (cWInterval);
    vLay8->setContentsMargins (0, 0, 0, 0);
    QHBoxLayout * hLay1 = new QHBoxLayout ();//cWInterval);
    hLay1->setContentsMargins (0, 0, 0, 0);
    lEIntervalValue = new QLineEdit (cWInterval);
    QIntValidator * pVal = new QIntValidator (1, 24, this);
    lEIntervalValue->setValidator (pVal);
    hLay1->addWidget (lEIntervalValue);
    cbUnits = new QComboBox (cWInterval);
    QStringList units = KKSAttrType::getIntervalUnits();
    for (int i=0; i<units.count(); i++)
        cbUnits->addItem (units[i], i+1);
    cbUnits->setCurrentIndex (5);
    hLay1->addWidget (cbUnits);
    vLay8->addLayout (hLay1);
    vLay8->addStretch ();
    stLayValue->addWidget (cWInterval);//insertWidget (7, cWInterval);

    //
    // Reference set
    //
    lvCheckRef = new QListView (wValue);
    lvCheckRef->viewport()->setSizePolicy(stp);
    checkRefModel = 0;//new KKSCheckableModel (0, 1, this);
    lvCheckRef->setModel (sortRefModel);//checkRefModel);
    sortRefModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    stLayValue->addWidget (lvCheckRef);

    //
    // Image attribute
    //
    QWidget * cWImage = new QWidget (wValue);
    cWImage->setSizePolicy(stp);
    QGridLayout * gLay1 = new QGridLayout (cWImage);
    gLay1->setContentsMargins (0, 0, 0, 0);
    wImage = new KKSImage (cWImage);
    wImage->setSizePolicy (QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    gLay1->addWidget (wImage, 0, 0, 2, 1, Qt::AlignJustify);
    tbImage = new QToolButton (cWImage);
    tbImage->setText (tr("..."));
    connect (tbImage, SIGNAL (clicked()), this, SLOT (loadImage()) );
    gLay1->addWidget (tbImage, 0, 1, 2, 1, Qt::AlignRight | Qt::AlignTop);
    stLayValue->addWidget (cWImage);//insertWidget (9, cWImage);
}

void KKSSearchTemplateForm :: viewSQL()
{
    if(ui->teSQLView->isVisible()){
        ui->teSQLView->setVisible(false);
        ui->pbShowSQL->setText(tr("Show SQL"));
    }
    else{
        ui->teSQLView->setVisible(true);
        ui->pbShowSQL->setText(tr("Hide SQL"));
    }
}

void KKSSearchTemplateForm :: addGroup (void)
{
    if(ui->rbAND->isChecked())
        createGroup(true);
    else
        createGroup(false);
    isDbSaved = false;
}

void KKSSearchTemplateForm :: addFilter ()
{
    QItemSelectionModel * sm = ui->twFilters->selectionModel();
    QModelIndex index;
    if(!sm || !(index = sm->currentIndex()).isValid()){
        addGroup ();
        const QAbstractItemModel * mod = sm->model ();
        index = mod->index (0, 0);
        sm->setCurrentIndex (index, QItemSelectionModel::Rows);
    }
    
    int cbIndex = cbAttribute->currentIndex();
    if(cbIndex < 0) 
        return;

    int id = cbAttribute->itemData (cbIndex).toInt();
    const KKSAttribute * a = 0;
    bool isSys;
    if (c->attributes().contains (id))
    {
        a = m_attrsAll.value (id);//c->attribute(id);
        isSys = true;
    }
    else
    {
        a = c->attribute (1);//m_attrsAll.value (id);
        isSys = false;
    }
    if (!a)
        return;

    cbIndex = cbOper->currentIndex();
    if(cbIndex < 0) 
        return;
    
    QVariant data = cbOper->itemData(cbIndex);
    KKSFilter::FilterOper oper = (KKSFilter::FilterOper) data.toInt();

    QString value;
    switch (stLayValue->currentIndex())
    {
        case 0: value = lEValue->text(); break;
        case 1: value = lEStrValue->text(); break;
/*        case 2: {
                    int index = cbValue->currentIndex();
                    value = cbValue->itemData (index).toString(); break;
                }
 */
        case 3: value = teValue->toPlainText(); break;
        case 4: value = QString ("%1").arg (chValue->checkState() == Qt::Checked ? QString("TRUE") : QString ("FALSE")); break;
        case 5: value = dValue->date().toString (Qt::ISODate); break;
        case 6: value = dtValue->dateTime ().toString (Qt::ISODate); break;
        case 7: value = QString ("{%1,%2}").arg (lEIntervalValue->text()).arg (cbUnits->itemData (cbUnits->currentIndex()).toInt()); break;
        case 8: 
                {
                    QAbstractItemModel * mod = lvCheckRef->model ();
                    if (a->type()->attrType() == KKSAttrType::atCheckList ||
                        a->type()->attrType() == KKSAttrType::atCheckListEx)
                    {
                        value = QString("{");
                        int num = 0;
                        for (int i=0; i < mod->rowCount(); i++)
                        {
                            QModelIndex wIndex = mod->index (i, 0);
                            if (mod->data (wIndex, Qt::CheckStateRole).toInt () == Qt::Checked)
                            {
                                if (num > 0 )//&& i < mod->rowCount()-1)
                                    value += ",";
                                value += QString::number (mod->data (wIndex, Qt::UserRole).toInt ());
                                num++;
                            }
                        }
                        value += "}";
                    }
                    else
                    {
                        QItemSelectionModel * selModel = lvCheckRef->selectionModel();
                        QModelIndexList selInds = selModel ? selModel->selectedIndexes() : QModelIndexList();
                        if (selInds.isEmpty())
                        {
                            value = QString();
                            break;
                        }
                        QModelIndex wIndex = selInds.at (0);
                        value = QString::number (wIndex.data(Qt::UserRole).toInt());
                    }
                    break;
                }
        case 9: 
               {
                   QImage cim = wImage->getImage ();
                   QByteArray imb;
                   QBuffer buffer (&imb);
                   buffer.open(QIODevice::WriteOnly);
                   cim.save (&buffer, "XPM");
                   value = QString (imb);
                   qDebug () << __PRETTY_FUNCTION__ << value;
                   break;
               }
        default: return;
    }

    KKSFilter * f = 0;
    if (c->attributes().contains (id))
        f = c->createFilter (a->id(), value, oper); // lEValue->text()
    else
    {
        KKSFilter::FilterOper operMain = KKSFilter::foInSQL;

        QString sql;
        bool cs = false;
        
        if ((stLayValue->currentIndex () == 1 && chCaseSensitive->checkState () == Qt::Checked) ||
            (stLayValue->currentIndex () == 3 && chTextCaseSensitive->checkState () == Qt::Checked))
        {
            cs = true;
        }
        else{
            cs = false;
        }

        QString operStr = cbOper->itemData (cbOper->currentIndex(), Qt::UserRole+1).toString();
        if(oper == KKSFilter::foLike || oper == KKSFilter::foLikeStart || oper == KKSFilter::foLikeIn || oper == KKSFilter::foLikeEnd){
            if(cs)
                operStr = "LIKE"; 
            else
                operStr = "ILIKE"; 
        }
        
        if(oper == KKSFilter::foLikeStart){
            value += "%";
        }
        else if(oper == KKSFilter::foLikeIn){
            value = "%" + value + "%";
        }
        else if(oper == KKSFilter::foLikeEnd){
            value = "%" + value;
        }

        if (id>0){
            
            sql = QString("select * from ioSearch('%1', %2, '%3')")
                            .arg(value)
                            .arg(id)
                            .arg (operStr);
        }
        else{//выбрана опция "поиск по всем атрибутам"
            
            sql = QString("select * from ioSearch('%1', '%2')")
                            .arg(value)
                            .arg (operStr);
        }
        
        f = c->createFilter (a->id(), sql, operMain);
    }
    
    if(!f){
        qCritical() << tr("Incorrect value for selected attribute and operation!");
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr("Incorrect value for selected attribute and operation!"), 
                              QMessageBox::Ok);
        return;
    }

    if(a->type()->attrType() == KKSAttrType::atParent){
        f->setRecursive(true);
    }

    if ((stLayValue->currentIndex () == 1 && chCaseSensitive->checkState () == Qt::Checked) ||
        (stLayValue->currentIndex () == 3 && chTextCaseSensitive->checkState () == Qt::Checked))
        f->setCaseSensitive(true);
    else
        f->setCaseSensitive(false);

    QAbstractItemModel * model = ui->twFilters->model();
    if(!model)
        return;

    index = index.sibling(index.row(), 0);
    if (index.data(Qt::UserRole+USER_ENTITY).toInt() == 0)
    { //filter selected
        index = index.parent();
        index = index.sibling(index.row(), 0);
    }
    else{//group selected
        ;
    }
    
    KKSFilterGroup * group = currentGroup();
    //KKSFilterGroup * group = const_cast<KKSFilterGroup*>(g);
    if(!group)
        return;

    bool isFilterC (false);
    for (int i=0; i<group->filters().count() && !isFilterC; i++)
        if ((*f) == (*group->filter(i)))
            isFilterC = true;
    if (isFilterC)
    {
        f->release ();
        return;
    }
    group->addFilter(f);

    int cnt = model->rowCount(index);
    
    model->insertRow(cnt, index);
    if(cnt == 0){
        if(model->columnCount(index) == 0)
            model->insertColumns(0, 4, index);
        ui->twFilters->setExpanded(index, true);
    }

    QString operType = index.sibling(index.row(), 3).data().toString();
    QString theAttr = isSys ? f->attribute()->title() : cbAttribute->currentText();
    int idAttr = isSys ? f->attribute()->id() : cbAttribute->itemData (cbAttribute->currentIndex()).toInt();
    QString theValue = value;
    if (isSys && f->value(0))
        theValue = f->value(0)->value();

    QModelIndex fIndex = model->index(cnt, 0, index);
    model->setData (fIndex, theAttr, Qt::DisplayRole);
    model->setData (fIndex, QVariant::fromValue (*f), Qt::UserRole);
    model->setData (fIndex, idAttr, Qt::UserRole+1);
    model->setData (model->index(cnt, 1, index), cbOper->currentText(), Qt::DisplayRole);
    model->setData (model->index(cnt, 2, index), theValue, Qt::DisplayRole);
    model->setData (model->index(cnt, 3, index), operType, Qt::DisplayRole);
    model->setData (model->index(cnt, 0, index), 0, Qt::UserRole+USER_ENTITY);//filter

    ui->twFilters->update(index);

    updateSQL();
    f->release();
    isDbSaved = false;
}

void KKSSearchTemplateForm :: setValueWidget (int index)
{
    qDebug () << __PRETTY_FUNCTION__ << index;

    if (index < 0 || index >= cbOper->count())
        return;

    QVariant data = cbOper->itemData(index);
    KKSFilter::FilterOper oper = (KKSFilter::FilterOper) data.toInt();
    switch(oper){
        case KKSFilter::foIsNotNull:
        case KKSFilter::foIsNull:
            //ui->leValue->setText(QString::null);
            wValue->setEnabled(false);
            break;
        default:
            wValue->setEnabled(true);
            break;
    }
}

void KKSSearchTemplateForm :: updateSQL()
{
    QString sql;
    QString tmp;

    if(m_filters.empty())
        return;
    
    QString tableName;
    
    for (int i=0; i<m_filters.count(); i++)
    {
        const KKSFilterGroup * group = m_filters[i];
        if(!group)
            continue;

        tmp = parseGroup(group, tableName);
        if(tmp.isEmpty())
            continue;

        if(i==0)
            sql += tmp;
        else
            sql += " and " + tmp;
    }

    ui->teSQLView->setText(sql);
}

QString KKSSearchTemplateForm :: parseGroup(const KKSFilterGroup * g, const QString & tableName)
{
    QString sql;
    QString tmp;
    
    if(!g)
        return sql;

    QString groupOper;
    
    tmp += " ( ";
    if(g->AND()){
        groupOper = " AND ";
    }
    else{
        groupOper = " OR ";
    }

    int grCount=0;
    const KKSList<const KKSFilterGroup*> groups = g->groups();
    if(groups.count() > 0)
    {
        for (int i=0; i<groups.count(); i++)
        {
            const KKSFilterGroup * group = groups[i];
            if(!group)
                continue;

            if(grCount == 0)
                tmp += parseGroup(group, tableName);
            else 
                tmp += groupOper + " " + parseGroup(group, tableName);
            if(tmp.isEmpty())
                continue;

            if(grCount==0)
                sql += tmp;
            else
                sql += groupOper + tmp;
            
            grCount++;
        }
    }
    
    tmp.clear();
    if(grCount==0){
        tmp = " ( ";
    }
    else{
        tmp = " " + groupOper + " ( ";
    }
    
    const KKSList<const KKSFilter*> filters = g->filters();
    if(filters.count() <= 0)
        return sql;

    int fCount;
    for (fCount=0; fCount < filters.count(); fCount++)
    {
        const KKSFilter * f = filters[fCount];
        if(!f || !f->isCorrect()){
            qWarning() << "KKSFilter is not correct!";
            continue;
        }

        if(fCount==0)
            tmp += parseFilter(f, tableName);
        else 
            tmp += groupOper + parseFilter(f, tableName);

    }
    
    //  tmp += " ) ";
    
    if(fCount>0)
        sql += tmp + " ) ";
    else
        sql += " ) ";
    
    if(grCount > 0)
        sql += " ) ";
    
    return sql;
}

QString KKSSearchTemplateForm :: parseFilter(const KKSFilter * f, const QString & tableName)
{
    QString sql;
    
    if(!f)
        return sql;

    const KKSAttribute * a = f->attribute();
    KKSList<const KKSValue *> values = f->values();
    KKSFilter::FilterOper oper = f->operation();
    bool cs = f->caseSensitive();
    
    //QString upper = QString(" upper(");
    //QString endUpper = QString(")");
    
    if(a->type()->attrType() != KKSAttrType::atString && 
        a->type()->attrType() != KKSAttrType::atFixString && 
        a->type()->attrType() != KKSAttrType::atText)
    {
        cs = true;
    }

    //Для случая если используется оператор LIKE
    QString sLike;
    QString aVal; //сюда потом будем добавлять символ %, если необходимо
    if(cs == true)
        sLike = "LIKE";
    else
        sLike = "ILIKE";


    QString code = a->code();


    if(tableName.isEmpty())
        sql += QString(" %1 ").arg(code);
    else
        sql += QString(" %1.%2 ").arg(tableName).arg(code);
    

    QString str;
    switch (oper){
        case KKSFilter::foEq:
            str += QString(" = %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foGr:
            str += QString(" > %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foLess:
            str += QString(" < %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foGrEq:
            str += QString(" >= %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foLessEq:
            str += QString(" <= %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foNotEq:
            str += QString(" <> %1").arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foIsNull:
            str += QString(" is null");
            break;
        case KKSFilter::foIsNotNull:
            str += QString(" is not null");
            break;
        case KKSFilter::foIn:
            str += QString(" in (%1)").arg(f->constructForIn());
            break;
        case KKSFilter::foNotIn:
            str += QString(" not in (%1)").arg(f->constructForIn());
            break;
        case KKSFilter::foInSQL:
            str += QString(" in (%1)").arg(values.at(0)->value());
            break;
        case KKSFilter::foBetween:
            str += QString(" between %1 and %2")
                        .arg(values.at(0)->valueForInsert())
                        .arg(values.at(1)->valueForInsert());
            break;
        case KKSFilter::foLike:
            str += QString(" %1 %2").arg(sLike).arg(values.at(0)->valueForInsert());
            break;
        case KKSFilter::foLikeIn:
            str += QString(" %1 %2").arg(sLike).arg("'%" + values.at(0)->value() + "%'");
            break;
        case KKSFilter::foLikeStart:
            str += QString(" %1 %2").arg(sLike).arg("'" + values.at(0)->value() + "%'");
            break;
        case KKSFilter::foLikeEnd:
            str += QString(" %1 %2").arg(sLike).arg("'%" + values.at(0)->value() + "'");
            break;
        default:
            break;
    }

    sql += str;

    return sql;
}

KKSList<const KKSFilterGroup*> & KKSSearchTemplateForm :: filters()
{
    return m_filters;
}

void KKSSearchTemplateForm :: setFilters (const KKSList<const KKSFilterGroup*> & filters)
{
    m_filters = filters;
    if (m_filters.isEmpty())
        return;
    const KKSFilterGroup * mainGroup = m_filters[0];
    QAbstractItemModel * mod = ui->twFilters->model();
    if (!mod)
    {
        mod = new QStandardItemModel (0, 4);
        ui->twFilters->setModel (mod);
    }
    int ngr = mod->rowCount ();
    mod->removeRows (0, ngr);
    if (mod->columnCount() < 4)
    {
        int ncol = mod->columnCount();
        mod->insertColumns (0, 4-ncol);
    }
    createGroup (mainGroup->AND());
    QModelIndex pIndex = mod->index (0, 0);
    QItemSelectionModel * sm = ui->twFilters->selectionModel ();
    sm->setCurrentIndex (pIndex, QItemSelectionModel::Select);
    //qDebug () << __PRETTY_FUNCTION__ << pIndex;
    setFiltersModel (mod, pIndex, mainGroup);
    updateSQL ();
}

void KKSSearchTemplateForm :: createGroup (bool AND)
{
    QItemSelectionModel * sm = ui->twFilters->selectionModel();
    if (!sm)
        return;
    QModelIndex index = sm->currentIndex ();
    if (!index.isValid())
    {
        index = QModelIndex();
    }
    else
    {
        index = index.sibling (index.row(), 0);
        if (index.data(Qt::UserRole+USER_ENTITY).toInt() == 0)
        {
            //
            // filter selected
            //
            index = index.parent ();
            index = index.sibling (index.row(), 0);
        }
        else
        {
            //
            // group selected
            //
            qDebug () << __PRETTY_FUNCTION__ << index;
        }
    }
    
    QAbstractItemModel * model = ui->twFilters->model();
    if(!model)
        return;

    int cnt = model->rowCount (index);
    if (!index.isValid () && cnt)
    {
        qWarning() << tr ("Cannot create another group in the top level");
        QMessageBox::warning (this, tr ("Create group"), tr ("Cannot create another group in the top level"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    
    model->insertRow(cnt, index);
    if(cnt == 0){
        if(model->columnCount(index) == 0)
            model->insertColumns(0, 4, index);
        ui->twFilters->setExpanded(index, true);
    }

    QString operType;
    if(AND)
        operType = tr("AND");
    else
        operType = tr("OR");

    model->setData (model->index(cnt, 0, index), tr("Group"), Qt::DisplayRole);
    model->setData (model->index(cnt, 1, index), "", Qt::DisplayRole);
    model->setData (model->index(cnt, 2, index), "", Qt::DisplayRole);
    model->setData (model->index(cnt, 3, index), operType, Qt::DisplayRole);
    model->setData (model->index(cnt, 0, index), 1, Qt::UserRole+USER_ENTITY);//group
    model->setData (model->index(cnt, 0, index), AND, Qt::UserRole+1); // is and

    KKSFilterGroup * group = new KKSFilterGroup(AND);

    KKSFilterGroup * g = currentGroup();
    if (!g)
    {
        m_filters.append (group);
        if (sTempl && !sTempl->getMainGroup())
            sTempl->setMainGroup (group);
    }
    else
        g->addGroup(group);
    
    group->release();

    ui->twFilters->setCurrentIndex(model->index(cnt, 0, index));
    ui->twFilters->update(index);
}

KKSFilterGroup * KKSSearchTemplateForm :: getGroup(QModelIndex index)
{
    
    if(index.parent().isValid()){
        KKSFilterGroup * g = getGroup(index.parent());
        if(index.data(Qt::UserRole+USER_ENTITY).toInt() == 0)//filter
            return g;
        
        QStandardItemModel * model = qobject_cast <QStandardItemModel* >(ui->twFilters->model());
        int rowCount = model->rowCount(index.parent());
        int row = 0;
        int currentRow = index.row();
        for(int i=0; i<rowCount; i++){
            QModelIndex ind = model->index(i, 0, index.parent());
            if(ind.data(Qt::UserRole+USER_ENTITY).toInt() == 1)//group
                row++;
            if(ind.row() == currentRow){
                row--;
                break;
            }
        }
        
        return const_cast<KKSFilterGroup*>(g->group(row));
    }
    
    return const_cast<KKSFilterGroup*>(m_filters.at(index.row()));
}

KKSFilterGroup * KKSSearchTemplateForm :: currentGroup()
{

    QItemSelectionModel * sm = ui->twFilters->selectionModel();
    QModelIndex index;
    if(!sm || !(index = sm->currentIndex()).isValid()){
        return NULL;
    }
    
    return getGroup(index);
}

void KKSSearchTemplateForm :: attrChanged (int index)
{
    KKSMap<int, KKSAttribute *>::const_iterator pa = index>=1 ? m_attrsAll.constBegin()+index-1 : m_attrsAll.constEnd();
    int idAttrType;
    if (pa !=  m_attrsAll.constEnd())
    {
        //qDebug () << __PRETTY_FUNCTION__ << index << pa.key() << pa.value()->name() << pa.value()->code() << (pa.value()->type()==0 ? -1 :  pa.value()->type() ->attrType());

        if (pa.value()->type()==0)
            return;
        idAttrType = pa.value()->type()->attrType();
    }
    else
        idAttrType = KKSAttrType::atString;

    this->initFilterTypes ((KKSAttrType::KKSAttrTypes)idAttrType);//pa.value()->type()->attrType());

    QSizePolicy textSP (QSizePolicy::Minimum, QSizePolicy::Minimum);
    switch (idAttrType)//pa.value()->type()->attrType())
    {
        case KKSAttrType::atString: 
        case KKSAttrType::atFixString: 
        case KKSAttrType::atUrl: 
                                     {
                                         stLayValue->setCurrentIndex (1); 
                                         break;
                                     }
        case KKSAttrType::atInt: 
        case KKSAttrType::atDouble: 
                                     {
                                         stLayValue->setCurrentIndex (0); 
                                         break;
                                     }
/*        case KKSAttrType::atList: 
        case KKSAttrType::atParent: 
                                     {
                                         QString tableName;
                                         
                                         if(pa.value()){
                                             tableName = pa.value()->tableName();
                                             if(tableName.isEmpty())
                                                 tableName = m_parentTable;
                                         }

                                         stLayValue->setCurrentIndex (2); 
                                         emit loadAttributeRefValues (tableName, pa.value(), cbValue);
                                         break;
                                     }
 */
        case KKSAttrType::atText: {
                                      int h = teValue->height ();
                                      stLayValue->setCurrentIndex (3);
                                      teValue->setSizePolicy (textSP);
                                      teValue->resize (250, h);
                                      teValue->viewport ()->resize (250, h);
                                      teValue->parentWidget()->resize (250, h);
                                      break;
                                  }
        case KKSAttrType::atBool: stLayValue->setCurrentIndex (4); break;
        case KKSAttrType::atDate: stLayValue->setCurrentIndex (5); break;
        case KKSAttrType::atDateTime: stLayValue->setCurrentIndex (6); break;
        case KKSAttrType::atInterval: stLayValue->setCurrentIndex (7); break;
        case KKSAttrType::atList: 
        case KKSAttrType::atParent: 
        case KKSAttrType::atCheckList: 
        case KKSAttrType::atCheckListEx: 
                                  {
                                      int h = lvCheckRef->height ();
                                      stLayValue->setCurrentIndex (8);
                                      lvCheckRef->setSizePolicy (textSP);
                                      lvCheckRef->resize (250, h);
                                      lvCheckRef->viewport ()->resize (250, h);
                                      lvCheckRef->parentWidget ()->resize (250, h);
                                      
                                      QString tableName;
                                      if(pa.value()){
                                          tableName = pa.value()->tableName();
                                          if(tableName.isEmpty())
                                              tableName = m_parentTable;
                                      }
                                      if (!checkRefModel)
                                          checkRefModel = (idAttrType == KKSAttrType::atList || idAttrType == KKSAttrType::atParent) ? new QStandardItemModel (0, 1) : new KKSCheckableModel (0, 1);
                                      else
                                      {
                                          QAbstractItemModel * oldModel = checkRefModel;
                                          checkRefModel = (idAttrType == KKSAttrType::atList || idAttrType == KKSAttrType::atParent) ? new QStandardItemModel (0, 1) : new KKSCheckableModel (0, 1);
                                          delete oldModel;
                                      }
                                      sortRefModel->setSourceModel (checkRefModel);
                                          
                                      emit loadAttributeRefValues (tableName, pa.value(), checkRefModel);
                                      sortRefModel->sort(0);
                                      
                                      break;
                                  }
        case KKSAttrType::atObjRef: break;
        case KKSAttrType::atJPG: stLayValue->setCurrentIndex (9); break;
        case KKSAttrType::atUndef: 
        default: stLayValue->setCurrentIndex (-1); break;
    }

    if (idAttrType == KKSAttrType::atInt ||
        idAttrType == KKSAttrType::atDouble)
    {
        lEValue->setValidator (0);
        if (pa.value()->type() ->attrType() == KKSAttrType::atInt)
        {
            QIntValidator * val = new QIntValidator (this);
            lEValue->setValidator (val);
        }
        else if (idAttrType == KKSAttrType::atDouble)
        {
            QDoubleValidator * val = new QDoubleValidator (this);
            lEValue->setValidator (val);
        }
    }
}

void KKSSearchTemplateForm :: setBoolValChanged (int state)
{
    bool val (state==Qt::Checked);

    if (val)
        chValue->setText (tr("is set"));
    else
        chValue->setText (tr("is not set"));
}

void KKSSearchTemplateForm :: loadImage (void)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Image File"), 
                                                    QDir::currentPath(),
                                                    tr("Images (*.png *.xpm *.jpg *.PNG *.XPM *.JPG)"));

    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        if (image.isNull())
        {
            qCritical() << tr("Cannot load %1.").arg(fileName);
            QMessageBox::critical(this, tr("Error"),
                                     tr("Cannot load %1.").arg(fileName));
            return;
        }

        wImage->setImage (image);
        wImage->update ();
        qDebug () << __PRETTY_FUNCTION__ << wImage->size ();
    }
}

void KKSSearchTemplateForm :: saveSQLQuery (void)
{
    if (sTempl)
        emit saveSearchCriteria (sTempl, sTempl->getMainGroup(), c);
    else if (!m_filters.isEmpty ())
        emit saveSearchCriteria (sTempl, const_cast<KKSFilterGroup *>(m_filters[0]), c);
    isDbSaved = true;
}

void KKSSearchTemplateForm :: loadSQLQuery (void)
{
    emit loadSearchCriteria ();//ui->twFilters->model());
}

void KKSSearchTemplateForm :: setFiltersModel (QAbstractItemModel * mod, const QModelIndex& parent, const KKSFilterGroup * parentGroup)
{
    if (!mod || !parent.isValid() || !parentGroup)
        return;

    int ngroups = parentGroup->groups().count ();
    int nfilt = parentGroup->filters().count ();
    //qDebug () << __PRETTY_FUNCTION__ << ngroups << nfilt;
    int nr = mod->rowCount (parent);
    int nc = mod->columnCount (parent);
    if (nr)
        mod->removeRows (0, nr, parent);
    if (nc)
        mod->removeColumns (0, nc, parent);
    mod->insertRows (0, ngroups+nfilt, parent);
    mod->insertColumns (0, 4, parent);

    for (int i=0; i<ngroups; i++)
    {
        const KKSFilterGroup * group = parentGroup->group (i);
        QString operType;
        if (group->AND())
            operType = tr("AND");
        else
            operType = tr("OR");
        mod->setData (mod->index(i, 0, parent), tr("Group"), Qt::DisplayRole);
        mod->setData (mod->index(i, 1, parent), "", Qt::DisplayRole);
        mod->setData (mod->index(i, 2, parent), "", Qt::DisplayRole);
        mod->setData (mod->index(i, 3, parent), operType, Qt::DisplayRole);
        mod->setData (mod->index(i, 0, parent), 1, Qt::UserRole+USER_ENTITY);//group
        mod->setData (mod->index(i, 0, parent), group->AND(), Qt::UserRole+1); // is and
        setFiltersModel (mod, mod->index (i, 0, parent),  group );
    }

    for (int i=0; i<nfilt; i++)
    {
        int row = i+ngroups;
        const KKSFilter * f = parentGroup->filter (i);
        QString operType;// = index.sibling(index.row(), 3).data().toString();
        switch (f->operation())
        {
            case KKSFilter::foEq: operType = "="; break;
            case KKSFilter::foGr: operType = ">"; break;
            case KKSFilter::foLess: operType = "<"; break;
            case KKSFilter::foGrEq: operType = ">="; break;
            case KKSFilter::foLessEq: operType = "<="; break;
            case KKSFilter::foLike: 
            case KKSFilter::foLikeIn:
            case KKSFilter::foLikeStart:
            case KKSFilter::foLikeEnd:
                operType = "like"; break;
            case KKSFilter::foNotEq: operType = "<>"; break;
            case KKSFilter::foIsNull: operType = "is null"; break;
            case KKSFilter::foIsNotNull: operType = "is not null"; break;
            case KKSFilter::foInSQL: operType = "in"; break;
            default: break;
        }
        QString theAttr = f ? f->attribute()->title() : QString();// : cbAttribute->currentText();
        int idAttr = f && f->attribute() ? f->attribute()->id() : -1;
        const KKSValue * val = f ? f->value (0) : 0;
        QString theValue = val ? val->value() : QString();
        qDebug () << __PRETTY_FUNCTION__ << theAttr << theValue;
        QModelIndex fIndex = mod->index (row, 0, parent);
        mod->setData (mod->index (row, 0, parent), theAttr, Qt::DisplayRole);
        mod->setData (fIndex, QVariant::fromValue (*f), Qt::UserRole);
        mod->setData (fIndex, idAttr, Qt::UserRole+1);
        mod->setData (mod->index (row, 1, parent), operType, Qt::DisplayRole);
        mod->setData (mod->index (row, 2, parent), theValue, Qt::DisplayRole);
        mod->setData (mod->index (row, 3, parent), parent.sibling(parent.row(), 3).data (Qt::DisplayRole).toString(), Qt::DisplayRole);
        mod->setData (mod->index (row, 0, parent), 0, Qt::UserRole+USER_ENTITY);//filter
    }
}

void KKSSearchTemplateForm :: saveSQLAccept (void)
{
    if (!isDbSaved)
        this->saveSQLQuery ();
    this->accept ();
}

KKSSearchTemplate * KKSSearchTemplateForm :: searchT (void) const
{
    return sTempl;
}

void KKSSearchTemplateForm :: setSearchTemplate (KKSSearchTemplate * st)
{
    if (sTempl)
        sTempl->release ();

    sTempl = st;

    if (sTempl)
        sTempl->addRef ();
}

void KKSSearchTemplateForm :: delFilter (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel *selModel = ui->twFilters->selectionModel ();
    if (!selModel)
        return;
    QItemSelection sel = selModel->selection ();
    if (sel.indexes().isEmpty())
        return;
    QModelIndex ind = sel.indexes().at (0);
    if (!ind.isValid ())
        return;
    ind = ind.sibling (ind.row(), 0);
    if (ind.data (Qt::UserRole+USER_ENTITY).toInt() > 0)
    {
        //
        // group
        //
        if (ind.parent().isValid ())
        {
            KKSFilterGroup * pGroup = getGroup (ind.parent());
            KKSFilterGroup * cGroup = getGroup (ind);
            cGroup->clear ();
            int grNum = -1;
            for (int i=0; i<pGroup->groups().count() && grNum <0; i++)
                if (cGroup->id() == pGroup->group(i)->id())
                    grNum = i;
            pGroup->removeGroup(grNum);//ind.row());
        }
        else
            m_filters.removeAt (0);
    }
    else
    {
        //
        // criterium
        //
        KKSFilterGroup * pGroup = getGroup (ind.parent());
        QString attrTitle = ind.data (Qt::DisplayRole).toString();
        qDebug () << __PRETTY_FUNCTION__ << attrTitle << (pGroup ? pGroup->id() : -1);
//        KKSCategoryAttr * cAttr = c->tableCategory()->attribute (attrCode);
        int filtNum = -1;
        QVariant vFilt = ind.data (Qt::UserRole);
        KKSFilter f = vFilt.value <KKSFilter>();
        for (int i=0; i<pGroup->filters().count() && filtNum < 0; i++)
        {
            //qDebug () << __PRETTY_FUNCTION__ << pGroup->filter(i)->attribute()->title() << attrTitle
            //          << pGroup->filter(i)->value(0)->value()
            //          << ind.sibling (ind.row(), 2).data(Qt::DisplayRole).toString();
            if (*pGroup->filter(i) == f)
                filtNum = i;
        }
        if (filtNum > 0)
            pGroup->removeFilter(filtNum);
    }
    QAbstractItemModel * fModel = ui->twFilters->model ();
    fModel->removeRows (ind.row(), 1, ind.parent());
    updateSQL();
}