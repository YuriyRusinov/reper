#include <QModelIndex>
#include <QFileDialog>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <QMessageBox>
#include <QtDebug>

#include "rubricform.h"
#include "ui_rubric_form.h"

#include <KKSSearchTemplate.h>
#include <KKSCategory.h>
#include <KKSAccessEntity.h>
#include "kksstuffform.h"
//#include "KKSCheckableModel.h"

RubricForm :: RubricForm (QString defaultRubricName, QString defaultRubricDesc, bool forRecords, QWidget* parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::rubric_form),
    searchTemplate (0),
    cat (0),
    stuffForm (0)
{
    UI->setupUi (this);
    UI->lERubricName->setText (defaultRubricName);
    UI->lERubricDescription->setText (defaultRubricDesc);

    UI->lSearchTemplate->setVisible (!forRecords);
    UI->lESearchTemplate->setVisible (!forRecords);
    UI->tbSearchTemplate->setVisible (!forRecords);

    UI->lCategory->setVisible (!forRecords);
    UI->lECategory->setVisible (!forRecords);
    UI->tbCategory->setVisible (!forRecords);

    UI->gbAccessRules->setVisible (!forRecords);
    if (forRecords)
        this->adjustSize ();
    //this->initPrivilegiesModel ();

    connect (UI->tbSearchTemplate, SIGNAL (clicked()), this, SLOT (loadSearchTemplate()) );
    connect (UI->tbCategory, SIGNAL (clicked()), this, SLOT (loadCategory ()) );
    connect (UI->tbRubricIcon, SIGNAL (clicked()), this, SLOT (loadRubricImage()) );

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

RubricForm :: ~RubricForm (void)
{
    delete UI;
}

QString RubricForm :: getRubricName (void) const
{
    return UI->lERubricName->text ();
}

void RubricForm :: setRubricName (QString rName)
{
    UI->lERubricName->setText (rName);
}

KKSSearchTemplate* RubricForm :: getSearchTemplate (void) const
{
    return searchTemplate;
}

void RubricForm :: setSearchTemplate (KKSSearchTemplate * st)
{
    UI->lESearchTemplate->clear ();
    if (searchTemplate)
        searchTemplate->release ();

    searchTemplate = st;

    if (searchTemplate)
    {
        searchTemplate->addRef ();
        UI->lESearchTemplate->setText (searchTemplate->name());
    }
}

KKSCategory * RubricForm :: getCategory (void) const
{
    return cat;
}

void RubricForm :: setCategory (KKSCategory * c)
{
    UI->lECategory->clear ();
    if (cat)
        cat->release ();

    cat = c;
    if (cat)
    { 
        cat->addRef ();
        UI->lECategory->setText (cat->name());
    }
}


void RubricForm :: loadSearchTemplate (void)
{
    emit requestSearchTemplate ();
}

void RubricForm :: loadCategory (void)
{
    emit requestCategory ();
}
/*
QAbstractItemModel * RubricForm :: getStuffModel (void) const
{
    return UI->tvStuffStruct->model ();
}

void RubricForm :: setStuffModel (QAbstractItemModel *stMod)
{
    QAbstractItemModel * oldModel = UI->tvStuffStruct->model ();
    UI->tvStuffStruct->setModel (stMod);
    if (oldModel && stMod != oldModel)
        delete oldModel;
}

void RubricForm :: initPrivilegiesModel (void)
{
    QStandardItemModel * privModel = new KKSCheckableModel (4, 1);
    QStringList privList;
    privList << tr ("Read of presense")
             << tr ("Read parameters")
             << tr ("Edit")
             << tr ("Remove");

    for (int i=0; i<privList.count(); i++)
    {
        QModelIndex wIndex = privModel->index (i, 0);
        privModel->setData (wIndex, privList[i], Qt::DisplayRole);
        privModel->setData (wIndex, Qt::Checked, Qt::CheckStateRole);
    }
    UI->lvPrivilegies->setModel (privModel);

    connect (UI->lvPrivilegies, SIGNAL (clicked (const QModelIndex&)),
             this,
             SLOT (currentPrivilegiesChanged (const QModelIndex&))
             );
}

void RubricForm :: currentPrivilegiesChanged (const QModelIndex& ind)
{
    qDebug () << __PRETTY_FUNCTION__ << ind;
}
*/

KKSStuffForm * RubricForm :: getStuffForm (void)
{
    return stuffForm;
}

void RubricForm :: setStuffForm (KKSStuffForm * _sForm)
{
    if (stuffForm)
    {
        stuffForm->setParent (0);
        delete stuffForm;
    }
    stuffForm = _sForm;
    stuffForm->setParent (UI->gbAccessRules);
    QGridLayout * gLay = qobject_cast<QGridLayout *>(UI->gbAccessRules->layout());
    if (!gLay)
    {
        UI->gbAccessRules->setLayout (0);
        gLay = new QGridLayout(UI->gbAccessRules);
    }
    gLay->addWidget (stuffForm, 0, 0, 1, 1);
//    connect (stuffForm, SIGNAL (accessRulesChanged(KKSAccessEntity *)), this, SLOT (setAccessEntity (KKSAccessEntity *)));
}

KKSAccessEntity * RubricForm :: getAccessEntity (void) const
{
    KKSAccessEntity * acl = stuffForm ? stuffForm->getAccessEntity() : 0;
    return acl;
}

void RubricForm :: setAccessEntity (KKSAccessEntity * _acl)
{
    if (stuffForm)
        stuffForm->setAccessEntity (_acl);
}

QString RubricForm :: getRubricDesc (void) const
{
    return UI->lERubricDescription->text();
}

void RubricForm :: setRubricDesc (QString rDesc)
{
    UI->lERubricDescription->setText (rDesc);
}

void RubricForm :: loadRubricImage (void)
{
    QString iconFile = QFileDialog::getOpenFileName(this, tr("Open Image File"),
                                                    QDir::currentPath(),
                                                    tr("Image files (*.xpm *.png *.ico *.jpg *.jpeg *.bmp *.gif *.pbm *.pgm *.xbm);;All files (*)")
                                                    );
    if (iconFile.isEmpty())
        return;

    QPixmap rubrPixmap (iconFile);
    if(rubrPixmap.isNull())
    {
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr(""), 
                              QMessageBox::Ok);
        return;
    }
    UI->lRubricIconImage->setPixmap(rubrPixmap);

}

QString RubricForm :: getIconAsString (void) const
{
    const QPixmap * rubrPixmap = UI->lRubricIconImage->pixmap();
    QByteArray bytes;
    QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);
    rubrPixmap->save(&buffer, "XPM"); // writes pixmap into bytes in XPM format
    buffer.close();
    
    return QString (bytes);
}

void RubricForm :: setIcon (const QPixmap& rubrIcon)
{
    UI->lRubricIconImage->setPixmap (rubrIcon);
}