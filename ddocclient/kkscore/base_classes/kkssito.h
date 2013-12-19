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
#ifndef KKSSITO_H
#define KKSSITO_H

#include "config_core.h"

#include <QSettings>
#include <QObject>
#include <QStringList>
#include <QTextStream>

#include <kkserror.h>
#include <KKSList.h>
#include "kksacclevel.h"

class KKSSettings;
class KKSDatabase;

//class KKSRecordFactory;
class KKSPPFactory;
class KKSEIOFactory;
class KKSLoader;
class KKSFileLoader;
class KKSObjEditorFactory;
class KKSCatEditorFactory;
class KKSTemplateEditorFactory;
class KKSRubricFactory;
class KKSStuffFactory;
class KKSAttributesFactory;
class KKSCmdJSettings;
class KKSMsgJSettings;
class KKSDbgOutputWidget;

class QWidget;
class QTextEdit;
class QTextCodec;
class QTranslator;

/*! \class KKSSito
  \ingroup SystemGroup
  \brief Класс инициализации библиотеки KKSSito
 
  Работа с библиотекой начинается с создания в приложении экземпляра данного класса. В настоящее время возможно существование только одного такого экземпляра. Данный класс инициализирует и возвращает указатель на экземпляр класса KKSDatabase (метод db()), а также выполняет ряд вспомогательных операций.
  

  Также данный класс предоставляет методы для вызова стандартных диалогов выбора файлов (getSaveFileName(), getOpenFileName(), getOpenFileNames()) и каталогов (getExistingDir()). Данные методы отличаются от стандартных вызовов QFileDialog::getOpenFileName(), QFileDialog::getOpenFileNames(), QFileDialog::getSaveFileName() и QFileDialog::getExistingDirectory() тем, что могут запоминать последний открытый каталог (метод getLastOpenedDir()) и фильтр расширений файлов. 
*/
class __CORE_EXPORT KKSSito : public QObject
{
    Q_OBJECT

private:
    KKSSito(const QString & userName = QString(), bool msgToWindow = true);
    virtual ~KKSSito();

public:
    KKSError * getLastError();  
    void showConnectionInfo(QWidget * parent = NULL) const;
    void showConnectionInfo(KKSDatabase * db, const QString & userName, const QString & dlName, const QString & macLabel, QWidget * parent = NULL) const;

    void updateLastError(KKSError::ErrorTypes type, 
                         QString code, 
                         QString name, 
                         QString message, 
                         QString group = QObject::tr("KKSSito client error group"));
    
    /*!\brief Метод возвращает каталог (абсолютный путь), открытый последним при помощи методов getSaveFileName(), getOpenFileName(), getOpenFileNames() в текущий сеанс работы. Если пользователь не использовал данные методы, то вернется текущий каталог (в данном случае его относительный путь).*/
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

    KKSCmdJSettings * getCmdJSettings();
    KKSMsgJSettings * getMsgJSettings();
    const KKSList<KKSAccLevel *> & getAccLevels() const;

    virtual KKSSettings * getKKSSettings();
    KKSDatabase * db() const;
    KKSDatabase * db1() const;//для отдельного потока
    //KKSRecordFactory * rf() const;
    KKSLoader * loader() const;
    KKSFileLoader * fileLoader() const;
    KKSPPFactory * ppf() const;
    KKSEIOFactory * eiof() const;
    KKSObjEditorFactory * oef() const;
    KKSAttributesFactory * attrf () const;
    KKSRubricFactory * rf () const;
    KKSCatEditorFactory * catf () const;
    KKSTemplateEditorFactory *tf () const;
    KKSStuffFactory *sf () const;
    //KKSIndFactory * indf () const;

    KKSDbgOutputWidget * dbgWidget(bool bCreateMenu = true, bool bForDockable = true) const;
    QTextStream & logStream();

private:
    KKSSettings * getKKSSettings (const QString & organization, 
                                  const QString & application = QString());
    KKSSettings * getKKSSettings (const QString & fileName);
    const QString & getWDir() const;

public:


    void loadTranslator();
    void loadQGISPlugins();
    const QString & GISHomeDir() const;

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

    /*!\brief Возвращает указатель на данный объект. 

    Как правило нет необходимости использовать данный метод. Необходимо просто где-то в начале программы (лучшим местом для этого является функция main()) создать экземпляр данного класса и в дальнейшем просто использовать глобальный указатель на KKSSito.
    \code
    #include <kkssito.h>

    void main(int argc, char*argv[])
    {
        KKSSito sito;
    }

    ...
    #include <kksdatabase.h>
    class MyClass
    {
        public:
            MyClass()
            {
                poDb = kksSito->db();
            }

            ~MyClass();
        private:
            KKSDatabase * poDb;
    };
    \endcode*/

    static KKSSito * instance() { return self; }
    static KKSSito * init (int argc, 
                           char *argv[],
                           bool with_connect = true, 
                           const QString & userName = QString(), 
                           bool msgToWindow = true);
    static int GUIConnect(QWidget * parent = NULL);


protected:
    KKSSettings * poSettings;

private:
    //
    // Variables
    //
    KKSError * lastError;
    mutable KKSDatabase * poDb;
    mutable KKSDatabase * poDb1;//для отдельного потока
    mutable KKSLoader * m_loader;
    mutable KKSFileLoader * m_fileLoader;
    mutable KKSPPFactory * m_ppf;
    mutable KKSEIOFactory * m_eiof;
    mutable KKSObjEditorFactory * m_objf;
//    mutable KKSAttrWidgetFactory * m_awf;
    mutable KKSRubricFactory * m_rf;
    mutable KKSCatEditorFactory * m_catf;
    mutable KKSTemplateEditorFactory * m_tf;
    mutable KKSStuffFactory * m_sf;
    mutable KKSAttributesFactory * m_attrf;
    mutable QString m_GISHomeDir; //каталог для временного хранения выгруженных на клиент файлов проектов и слоев QGIS
    //mutable KKSIndFactory * m_indf;

    mutable KKSDbgOutputWidget * m_dbgWidget;
    QTextStream m_logStream;

    static KKSSito * self;
    QTranslator * tor;
    QString allowedUserName; //хранит имя пользователя, под которым разрешен доступ в систему
                             //задается в методе init(). Если пусто, то войти можно под любым пользователем

#ifdef _FOR_LESSONS_SHEDULER_
    QTranslator * tor_sh;
    QTranslator * tor_su;
#endif

    QString lastOpenedDir;
    QString lastSelectedFilter;
    QString workingDir;
    KKSCmdJSettings * m_savedCmdJSettings;
    KKSMsgJSettings * m_savedMsgJSettings;
    KKSList<KKSAccLevel *> m_accLevels;

    void loadLastOpenedDir();
    void loadLastSelectedFilter();
    void loadCmdJSetings();
    void loadMsgJSetings();
    void loadAccLevels();
    void loadDefAccLevels();

    void initFactories();
    void initLogStream();

private slots:
    
    void saveCmdJSettings(KKSCmdJSettings * settings);
    void saveMsgJSettings(KKSMsgJSettings * settings);
    static void saveAccLevels(const KKSList<KKSAccLevel*> & levels);
    void clearLoader();

};

#define kksSito (static_cast<KKSSito *>(KKSSito::instance()))

const QString kksSitoName = QObject::tr("DynamicDocs Client");

#endif
