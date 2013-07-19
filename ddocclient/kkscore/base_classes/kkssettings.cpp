#include <QtCore>
#include <QtGui>

#include "kkssito.h"
#include "kkssettings.h"
#include "savesettingsform.h"

KKSSettings :: KKSSettings ( const QString & organization, const QString & application, QObject * parent /*= NULL*/ ) :
    QSettings (organization, application, parent)
{
}

KKSSettings :: KKSSettings ( const QString & fileName, Format format /*= QSettings::IniFormat*/, QObject * parent/*=NULL*/ ) :
    QSettings ( fileName, format, parent )
{
}

KKSSettings :: KKSSettings ( QObject * parent/*=NULL*/ ) :
    QSettings (parent)
{
}

KKSSettings :: ~KKSSettings ( void )
{
}

/* !\brief ����� ������������ ����� ��������� � ������� ������ �� ��� �����, 
 ���� �������� �� ������, �� ������������ ������ ������.
*/ 

QString KKSSettings :: getParam ( const QString& keyname ) const
{
    QString wstr = this->value (keyname).toString ();
    //qDebug () << "key = " << keyname << " value = " << wstr;
    return wstr;
}

/* !\brief ����� ������������ ����� ��������� � �������� ������ �� ��� �����, 
 ���� �������� �� ������, �� ������������ ������ ������.
*/

QString KKSSettings :: getGroupParam ( const QString& group, const QString& keyname )
{
    this->beginGroup (group);
    QString val = this->getParam (keyname);
    this->endGroup ();
    return val;
}

/*! \brief ����� ������������ �������������� �������� ����� ����������� �����.

� �������� ��������� ����������� ��������� �� ������������ ����.
 */

void KKSSettings :: editSettings ( QWidget * parent )
{
    SaveSettingsForm * setForm = new SaveSettingsForm (this, parent);
    if ( setForm->exec() == QDialog::Accepted )
    {
        
        QMessageBox::information ( 0, tr("Save settings"),
                tr("You have to restart your application in order to changes take effect"),
                QMessageBox::Ok );
        kksSito->loadTranslator();
    }
    delete setForm;
}

/*! \brief ����� ������ �������� \a val ��������� \a key �� ������ \a group
*/

void KKSSettings :: writeSettings ( const QString & group, const QString & key, const QString & val )
{
    this->beginGroup (group);
    this->setValue (key, val);
    this->endGroup ();
}

/*! \brief ����� ������������ �������������� �������� ��� ���������� ������� ����������,
 ����������� � KKSSito*/

void KKSSettings :: readSettings ( void )
{
    QStringList groups = this->childGroups();
    for (int i=0; i<groups.count(); i++)
    {
        this->beginGroup ( groups[i] );
        if (this->childGroups ().count () > 0)
        {
            qDebug () << __PRETTY_FUNCTION__ << QString ("Sub group");
            readSettings ();
        }
        this->endGroup ();
        QStringList keys = this->childKeys ();
        for (int ii=0; ii<keys.count(); ii++)
            this->setValue ( keys[ii], this->value (keys[ii]));
    }
}
