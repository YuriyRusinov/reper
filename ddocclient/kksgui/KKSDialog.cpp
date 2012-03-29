#include <QEventLoop>
#include <QPointer>

#include "KKSDialog.h"

KKSDialog :: KKSDialog (QWidget *parent, Qt::WindowFlags f)
    : QDialog (parent, f),
    pEventL (0)//new QEventLoop())
{
}

KKSDialog :: ~KKSDialog (void)
{
    if (pEventL && pEventL->isRunning())
        pEventL->exit (0);

    if (pEventL)
    {
        if (pEventL && pEventL->isRunning())
            pEventL->exit (0);
        delete pEventL;
        pEventL = 0;
    }
}

int KKSDialog :: exec (void)
{
    return QDialog::exec ();
/*    if (pEventL)
    {
        qWarning("QDialog::exec: Recursive call detected");
        return -1;
    }
    bool deleteOnClose = testAttribute(Qt::WA_DeleteOnClose);
    setAttribute (Qt::WA_DeleteOnClose, false);

    bool wasShowModal = testAttribute(Qt::WA_ShowModal);
    setAttribute (Qt::WA_ShowModal, true);
    res = 0;

    this->show ();
    if (pEventL)
    {
        if (pEventL->isRunning())
            pEventL->exit (0);
        delete pEventL;
        pEventL = 0;
    }
    pEventL = new QEventLoop();//&eventLoop;
    QPointer<QWidget> guard = this;

    if (!guard.isNull () && pEventL)
        pEventL->exec();

    if (guard.isNull())
    {
        pEventL = 0;
        return QDialog::Rejected;
    }
    setAttribute(Qt::WA_ShowModal, wasShowModal);

//    int res = result();
    if (deleteOnClose)
        delete this;
    return res;
*/
}

void KKSDialog :: accept (void)
{
    return QDialog::accept();
/*    done (QDialog::Accepted);
    close ();
*/
}

void KKSDialog :: reject (void)
{
    return QDialog::reject ();
//    done (QDialog::Rejected);
//    close ();
}

void KKSDialog :: done (int r)
{
    QDialog::done (r);
/*    hide ();
    res = r;
    emit finished (r);
    if (pEventL && pEventL->isRunning())
        pEventL->exit (0);

    if (r == QDialog::Accepted)
        emit accepted();
    else if (r == QDialog::Rejected)
        emit rejected();
*/
}

int KKSDialog :: result (void) const
{
    return QDialog::result();
}

void KKSDialog::closeEvent (QCloseEvent * event)
{
    emit aboutToClose();

    QWidget::closeEvent (event);
}
