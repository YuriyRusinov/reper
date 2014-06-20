/***************************************************************************
 *   Copyright (C) 2004 by NIL-5                                           *
 *   oleg.semykin@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef KKSCOREAPPLICATION_H
#define KKSCOREAPPLICATION_H

#include "config_coreapplication.h"

#include <QSettings>
#include <QObject>
#include <QStringList>
#include <QTextStream>

#include <kkserror.h>
#include <KKSList.h>
#include "kksacclevel.h"

class KKSSettings;
class KKSDatabase;
class KKSPluginLoader;

//class KKSRecordFactory;
class KKSPPFactory;
class KKSEIOFactory;
class KKSLoader;
class KKSFileLoader;

class KKSDbgOutputWidget;

class QWidget;
class QTextEdit;
class QTextCodec;
class QTranslator;
class KKSCommandLineOpts;

/*! \class KKSApplication
  \ingroup SystemGroup
  \brief ����� ������������� ���������� DynamicDocs Client
 
  ������ � ����������� ���������� � �������� � ���������� ���������� ������� ������. � ��������� ����� �������� ������������� ������ ������ ������ ����������. ������ ����� �������������� � ���������� ��������� �� ��������� ������ KKSDatabase (����� db()), � ����� ��������� ��� ��������������� ��������.
  

  ����� ������ ����� ������������� ������ ��� ������ ����������� �������� ������ ������ (getSaveFileName(), getOpenFileName(), getOpenFileNames()) � ��������� (getExistingDir()). ������ ������ ���������� �� ����������� ������� QFileDialog::getOpenFileName(), QFileDialog::getOpenFileNames(), QFileDialog::getSaveFileName() � QFileDialog::getExistingDirectory() ���, ��� ����� ���������� ��������� �������� ������� (����� getLastOpenedDir()) � ������ ���������� ������. 
*/
class __COREAPP_EXPORT KKSCoreApplication : public QObject
{
    Q_OBJECT

protected:
    KKSCoreApplication(KKSCommandLineOpts * opts, bool msgToWindow = true);
    virtual ~KKSCoreApplication();

public:
    KKSError * getLastError();  
    const KKSCommandLineOpts * commandLineOpts() const;
    void showConnectionInfo(QWidget * parent = NULL) const;
    void showConnectionInfo(KKSDatabase * db, 
                            const QString & userName, 
                            const QString & dlName, 
                            const QString & macLabel, 
                            QWidget * parent = NULL) const;
    
    void updateLastError(KKSError::ErrorTypes type, 
                         QString code, 
                         QString name, 
                         QString message, 
                         QString group = QObject::tr("KKSSito client error group"));
    
    /*!\brief ����� ���������� ������� (���������� ����), �������� ��������� ��� ������ ������� getSaveFileName(), getOpenFileName(), getOpenFileNames() � ������� ����� ������. ���� ������������ �� ����������� ������ ������, �� �������� ������� ������� (� ������ ������ ��� ������������� ����).*/
    QString getLastOpenedDir(){return lastOpenedDir;}
    QString getExistingDir(QWidget * parent = 0, 
                           QString caption = QString(), 
                           QString startDir = QString());

    QString getSaveFileName(QWidget * parent = 0, 
                            QString caption = QString(), 
                            QStringList filters = QStringList(), 
                            QString * selectedFilter = NULL, 
                            QString startDir = QString());
    
    QString getOpenFileName(QWidget * parent = 0, 
                            QString caption = QString(), 
                            QStringList filters = QStringList(), 
                            QString startDir = QString(), 
                            QString startFilter = QString());
    
    QStringList getOpenFileNames(QWidget * parent = 0, 
                                 QString caption = QString(), 
                                 QStringList filters = QStringList(), 
                                 QString startDir = QString(), 
                                 QString startFilter = QString());

    const KKSList<KKSAccLevel *> & getAccLevels() const;

    virtual KKSSettings * getKKSSettings();
    KKSDatabase * db() const;
    KKSDatabase * db1() const;//��� ���������� ������

    KKSLoader * loader() const;
    KKSFileLoader * fileLoader() const;
    KKSPluginLoader * pluginLoader() const;
    KKSPPFactory * ppf() const;
    KKSEIOFactory * eiof() const;

    KKSDbgOutputWidget * dbgWidget(bool bCreateMenu = true, bool bForDockable = true) const;
    QTextStream & logStream();

    const QString & getAllowedUserName() const {return allowedUserName;}
    void setAllowedUserName(const QString & name) {allowedUserName = name;}

protected:
    KKSSettings * getKKSSettings (const QString & organization, 
                                  const QString & application = QString());
    KKSSettings * getKKSSettings (const QString & fileName);
    const QString & getWDir() const;

public:


    void loadTranslator();
    void loadKKSPlugins();

    QByteArray toCString(QString str)
    {
        QByteArray ba = str.toLocal8Bit();
        return ba;
    }
    QString toQString(const char * str)
    {
        QString s = QString::fromLocal8Bit(str);
        return s;
    }

    /*!\brief ���������� ��������� �� ������ ������. 

    ��� ������� ��� ������������� ������������ ������ �����. ���������� ������ ���-�� � ������ ��������� (������ ������ ��� ����� �������� ������� main()) ������� ��������� ������� ������ � � ���������� ������ ������������ ���������� ��������� �� KKSApplication.
    \code
    #include <kksapplication.h>

    void main(int argc, char*argv[])
    {
        KKSApplication app;
    }

    ...
    #include <kksdatabase.h>
    class MyClass
    {
        public:
            MyClass()
            {
                poDb = kksApp->db();
            }

            ~MyClass();
        private:
            KKSDatabase * poDb;
    };
    \endcode*/


    static KKSCoreApplication * instanceCore() { return selfCore; }

    static KKSCoreApplication * init (int argc, 
                                      char *argv[],
                                      //bool with_connect = true, 
                                      //const QString & userName = QString(), 
                                      bool msgToWindow = true);


    static int GUIConnect(QWidget * parent = NULL);
    static int autoConnect(QWidget * parent = NULL);
    static int connectIfReady();
    static int verifyConnection(QWidget * parent = NULL);

    static KKSCommandLineOpts * parseCommandLineOptions(int argc, char * argv[]);
    static void showCommandLineParamsHelp(QWidget * parent = NULL);


protected:
    KKSSettings * poSettings;

private:
    //
    // Variables
    //
    KKSError * lastError;
    mutable KKSDatabase * poDb;
    mutable KKSDatabase * poDb1;//��� ���������� ������

    mutable KKSCommandLineOpts * m_kksOpts;// ��������� ��������� ������, ���������� � ����������

    mutable KKSLoader * m_loader;
    mutable KKSPluginLoader * m_pluginLoader;
    mutable KKSFileLoader * m_fileLoader;
    mutable KKSPPFactory * m_ppf;
    mutable KKSEIOFactory * m_eiof;

    mutable KKSDbgOutputWidget * m_dbgWidget;
    QTextStream m_logStream;

    static KKSCoreApplication * selfCore;
    QTranslator * tor;
    QString allowedUserName; //������ ��� ������������, ��� ������� �������� ������ � �������
                             //�������� � ������ init(). ���� �����, �� ����� ����� ��� ����� �������������

    QString lastOpenedDir;
    QString lastSelectedFilter;
    QString workingDir;
    KKSList<KKSAccLevel *> m_accLevels;

    void loadLastOpenedDir();
    void loadLastSelectedFilter();
    void loadAccLevels();
    void loadDefAccLevels();

    void initCoreFactories();
    void initLogStream();

private slots:
    
    static void saveAccLevels(const KKSList<KKSAccLevel*> & levels);
    void clearLoader();

};

#define kksCoreApp (static_cast<KKSCoreApplication *>(KKSCoreApplication::instanceCore()))

const QString kksAppName = QObject::tr("DynamicDocs Client");

#endif
