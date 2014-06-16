#ifndef SENDASXMLFORM_H
#define SENDASXMLFORM_H

namespace Ui
{
    class send_as_xml_form;
}

#include <QDialog>

class KKSObject;
class KKSObjectExemplar;
class KKSRecWidget;

class SendAsXMLForm : public QDialog
{
    Q_OBJECT
public:
    SendAsXMLForm( QWidget * parent = 0, Qt::WFlags f = 0);
    ~SendAsXMLForm();
    

public slots:
    void slotPbSendClicked();
    void slotPbCancelClicked();
    void setIO(KKSObject * io);
    void setEIO(KKSObjectExemplar * eio);

private:

    Ui::send_as_xml_form * ui;
    KKSRecWidget * rec;

    KKSObject * m_io;
    KKSObjectExemplar * m_eio;
};

#endif
