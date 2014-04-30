#ifndef DIGITALSIGNATUREFORM_H
#define DIGITALSIGNATUREFORM_H

namespace Ui
{
    class digital_signature_form;
}

#include <QDialog>

class DigitalSignatureForm : public QDialog
{
    Q_OBJECT
public:
    DigitalSignatureForm( QWidget * parent = 0, Qt::WFlags f = 0);
    ~DigitalSignatureForm();
    

public slots:
    void slotPbOkClicked();

private:

    Ui::digital_signature_form * ui;
};

#endif
