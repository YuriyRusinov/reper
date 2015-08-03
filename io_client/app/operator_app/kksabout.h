#ifndef FBLOG_FBAbout_H
#define FBLOG_FBAbout_H

namespace Ui
{
    class KKSAboutDlg;
}

#include <QDialog>

class KKSAbout : public QDialog
{
    Q_OBJECT
public:
    KKSAbout( QWidget * parent = 0, Qt::WFlags f = 0);
    ~KKSAbout();
    

public slots:
    void slotPbOkClicked();

private:

    Ui::KKSAboutDlg * ui;
};

#endif
