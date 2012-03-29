#include "choosetemplateform.h"
#include "ui_choose_template_form.h"
#include <QMessageBox>
#include <QTreeWidget>

ChooseTemplateForm::ChooseTemplateForm(const KKSList<KKSTemplate*> & tList, QWidget *parent)
    : QDialog(parent)
{
    ui = new Ui::choose_template_form;
    ui->setupUi(this);
    
    m_tList = tList;
    init();
}

ChooseTemplateForm::~ChooseTemplateForm()
{

}

KKSTemplate * ChooseTemplateForm::getCurrentTemplate()
{
    KKSTemplate * t = NULL;
    int row = ui->twTemplates->currentIndex().row();
    if(row < 0 || row >= m_tList.count())
        return t;

    t = m_tList.at(row);

    return t;
}

void ChooseTemplateForm::init()
{
    for(int i=0; i<m_tList.count(); i++){
        QStringList strings;
        strings << m_tList.at(i)->name();
        QTreeWidgetItem * item = new QTreeWidgetItem(ui->twTemplates, strings);
        QVariant data = QVariant(m_tList.at(i)->id());
        item->setData(0, Qt::UserRole, data);
    }
}

void ChooseTemplateForm::on_pbOk_clicked()
{
    if(!ui->twTemplates->currentItem()){
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr("You should select one template!"), 
                              QMessageBox::Ok);
        return;
    }
    accept();
}

void ChooseTemplateForm::on_pbCancel_clicked()
{
    reject();
}

bool ChooseTemplateForm::isSave (void) const
{
    bool isChecked (ui->chSave->checkState () == Qt::Checked);
    return isChecked;
}
