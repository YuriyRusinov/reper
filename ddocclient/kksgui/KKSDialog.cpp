#include <QEventLoop>
#include <QPointer>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "KKSDialog.h"

KKSDialog :: KKSDialog (QWidget *parent, Qt::WindowFlags f)
    : QDialog (parent, f),
    pEventL (0)//new QEventLoop())
{
    m_icon = QIcon(":/ddoc/rubric_item.png");
}

KKSDialog :: ~KKSDialog (void)
{
/*    if (pEventL && pEventL->isRunning())
        pEventL->exit (0);

    if (pEventL)
    {
        if (pEventL && pEventL->isRunning())
            pEventL->exit (0);
        delete pEventL;
        pEventL = 0;
    }
 */
}

QSize KKSDialog::sizeHint() const
{
    QWidget * parent = parentWidget();
    if(!parent)
        return QSize(500, 500);

    QString parentName = QString(parent->metaObject()->className());
    if(parentName == QString("QMdiSubWindow")){
        QMdiSubWindow * w = qobject_cast<QMdiSubWindow *>(parent);
        if(!w)
            return QSize(500, 500);
        QMdiArea *a = w->mdiArea();
        if(!a)
            return QSize(500, 500);
        return a->size();
    }

    QSize s = parent->size();
    return s;
}

/*
int KKSDialog :: exec (void)
{
    return QDialog::exec ();
}
 */

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
    emit aboutToClose(this);

    QWidget::closeEvent (event);
}

const QIcon & KKSDialog::icon() const
{
    return m_icon;
}
