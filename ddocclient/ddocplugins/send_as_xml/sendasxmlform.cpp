#include "sendasxmlform.h"
#include "ui_send_as_xml_form.h"


#include <QMessageBox>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <KKSObject.h>
#include <KKSObjectExemplar.h> 
#include <kksapplication.h>
#include <kkscoreapplication.h>
#include <KKSRecWidget.h>

#include <KKSLoader.h>
#include <KKSPPFactory.h>
#include <KKSEIOFactory.h>

#include <KKSFilter.h>
#include <KKSViewFactory.h>
#include <KKSTemplate.h>
#include <KKSList.h>
#include <KKSObject.h>
#include <KKSCategory.h>
#include <defines.h>

SendAsXMLForm::~SendAsXMLForm(void)
{
}



SendAsXMLForm::SendAsXMLForm(QWidget * parent,  Qt::WFlags f) : QDialog(parent, f)
{
    m_io = NULL;
    m_eio = NULL;
    rec = NULL;

    ui = new Ui::send_as_xml_form;
    ui->setupUi(this);

    connect(ui->pbSend, SIGNAL(clicked()), this, SLOT(slotPbSendClicked()));
    connect(ui->pbCancel, SIGNAL(clicked()), this, SLOT(slotPbCancelClicked()));

    if(!kksApp)
        return;

    const KKSList<const KKSFilterGroup *> filters;
     
    KKSObject * obj = NULL;
    obj = kksApp->loader()->loadIO(IO_ORG_ID, true);
    if(!obj)
        return;

    KKSCategory * c = obj->category()->tableCategory();
    KKSTemplate * t = new KKSTemplate(-1, QObject::tr("Organizations"), c);

    KKSMap<int, KKSAttrView *> a;
    int order = 0;

    KKSCategoryAttr * ca = c->attribute("id");
    KKSAttrView * av = new KKSAttrView(*ca);
    av->setOrder(order++);
    av->setDefWidth(50);
    a.insert(ca->id(), av);
    av->release();

    ca = c->attribute("name");
    av = new KKSAttrView(*ca);
    av->setOrder(order++);
    av->setDefWidth(250);
    a.insert(ca->id(), av);
    av->release();

    ca = c->attribute("short_name");
    av = new KKSAttrView(*ca);
    av->setOrder(order++);
    av->setDefWidth(150);
    a.insert(ca->id(), av);
    av->release();

    ca = c->attribute("id_parent");
    av = new KKSAttrView(*ca);
    av->setOrder(order++);
    av->setDefWidth(150);
    a.insert(ca->id(), av);
    av->release();

    KKSAttrGroup * g = new KKSAttrGroup();
    
    g->setAttrViews(a);
    t->addGroup(g);
    g->release();
  
    
    rec = KKSViewFactory::createView(t, obj, kksApp->loader(), filters, ui->frame);
    if(!rec)
        return;
    
    rec->actAdd->setVisible(false);
    rec->actDel->setVisible(false);
    rec->actEdit->setVisible(false);
    rec->actExport->setVisible(false);
    rec->actFilter->setVisible(false);
    rec->actGroupBy->setVisible(false);
    rec->actImport->setVisible(false);
    rec->actRefresh->setVisible(false);
    rec->actSearchT->setVisible(false);
    rec->actSetView->setVisible(false);
    rec->actViewAll->setVisible(false);
    rec->actEditSep->setVisible(false);
    rec->actFilterSep->setVisible(false);
    rec->actImportExportSep->setVisible(false);

    QGridLayout * layout = new QGridLayout(ui->frame);
    layout->setSpacing(0);
    ui->frame->setLayout(layout);
    layout->addWidget(rec);
    t->release();
    obj->release();

}


void SendAsXMLForm::slotPbSendClicked()
{
    if(!rec)
        return;

    QList<qint64> ids = rec->getIDList();
    if(ids.isEmpty()){
        QMessageBox::information(this, tr("Send As XML"), tr("You should select at least one organization"), QMessageBox::Ok);
        return;
    }
    
    int idObject = 0;
    qint64 idRecord = 0;
    
    if(m_io){
        idObject = m_io->id();

        for(int i=0; i<ids.count(); i++){
            int idOrg = ids.at(i);
            int idQueue = kksApp->ppf()->sendIOToExternalSystem(idOrg, ID_INTERACTION_FORMAT_PARAM_VALUE, idObject);
            if(idQueue <= 0){
                QMessageBox::critical(this, tr("Error"), tr("Cannot send selected data to external system!"), QMessageBox::Ok);
                return;
            }
        }
    }
    else if(m_eio){
        idRecord = m_eio->id();
        
        if(!m_eio->io()){
            QMessageBox::critical(this, tr("Send As XML"), tr("Inconsistent record found! Cannot send!"), QMessageBox::Ok);
            return;
        }
        
        idObject = m_eio->io()->id();

        for(int i=0; i<ids.count(); i++){
            int idOrg = ids.at(i);
            int idQueue = kksApp->eiof()->sendEIOToExternalSystem(idOrg, ID_INTERACTION_FORMAT_PARAM_VALUE, idObject, idRecord);
            if(idQueue <= 0){
                QMessageBox::critical(this, tr("Error"), tr("Cannot send selected data to external system!"), QMessageBox::Ok);
                return;
            }
        }
    }
    else{
        QMessageBox::critical(this, tr("Send As XML"), tr("Nothing to send..."), QMessageBox::Ok);
        return;
    }


    
    accept();
}

void SendAsXMLForm::slotPbCancelClicked()
{
    reject();
}

void SendAsXMLForm::setIO(KKSObject * io)
{
    if(!io)
        return;

    if(m_io)
        m_io->release();
    m_io = io;
    if(m_io)
        m_io->addRef();

    ui->leName->setText(io->name());
}

void SendAsXMLForm::setEIO(KKSObjectExemplar * eio)
{
    if(!eio)
        return;

    if(m_eio)
        m_eio->release();
    m_eio = eio;
    if(m_eio)
        m_eio->addRef();

    ui->leName->setText(eio->name());
}
