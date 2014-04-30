#include "digitalsignatureform.h"
#include "ui_digital_signature_form.h"


#include <QMessageBox>
#include <QListWidget>
#include <QTreeWidgetItem>

DigitalSignatureForm::~DigitalSignatureForm(void)
{
}



DigitalSignatureForm::DigitalSignatureForm(QWidget * parent,  Qt::WFlags f) : QDialog(parent, f)
{
    ui = new Ui::digital_signature_form;
    ui->setupUi(this);

    connect(ui->pbOK, SIGNAL(clicked()), this, SLOT(slotPbOkClicked()));
}


void DigitalSignatureForm::slotPbOkClicked()
{
    accept();
}
