#include "kksabout.h"
#include "ui_kksaboutdlg.h"
#include "kksclient_name.h"
#include <QRadioButton>
#include <QButtonGroup>

KKSAbout::KKSAbout(QWidget * parent,  Qt::WFlags f) 
: QDialog(parent, f)
{
    ui = new Ui::KKSAboutDlg;
    ui->setupUi(this);

    connect(ui->pbOK, SIGNAL(clicked()), this, SLOT(slotPbOkClicked()));

    QString s = tr("Version %1").arg(KKS_VERSION);
    ui->lProductVersion->setText(s);
}

KKSAbout::~KKSAbout()
{

}

void KKSAbout::slotPbOkClicked()
{
    accept();
}
