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
#ifndef KKSAPPLICATION_H
#define KKSAPPLICATION_H

#include "config_application.h"
#include <kkscoreapplication.h>

#include <QObject>

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

//class QWidget;
//class QTextEdit;
//class QTextCodec;
//class QTranslator;

/*! \class KKSApplication
  \ingroup SystemGroup
  \brief Класс инициализации библиотеки DynamicDocs Client
 
  Работа с библиотекой начинается с создания в приложении экземпляра данного класса. В настоящее время возможно существование только одного такого экземпляра. Данный класс инициализирует и возвращает указатель на экземпляр класса KKSDatabase (метод db()), а также выполняет ряд вспомогательных операций.
  

  Также данный класс предоставляет методы для вызова стандартных диалогов выбора файлов (getSaveFileName(), getOpenFileName(), getOpenFileNames()) и каталогов (getExistingDir()). Данные методы отличаются от стандартных вызовов QFileDialog::getOpenFileName(), QFileDialog::getOpenFileNames(), QFileDialog::getSaveFileName() и QFileDialog::getExistingDirectory() тем, что могут запоминать последний открытый каталог (метод getLastOpenedDir()) и фильтр расширений файлов. 
*/
class __KKSAPP_EXPORT KKSApplication : public KKSCoreApplication
{
    Q_OBJECT

private:
    KKSApplication(const QString & userName = QString(), bool msgToWindow = true);
    virtual ~KKSApplication();

public:

    KKSObjEditorFactory * oef() const;
    KKSAttributesFactory * attrf () const;
    KKSRubricFactory * rf () const;
    KKSCatEditorFactory * catf () const;
    KKSTemplateEditorFactory *tf () const;
    KKSStuffFactory *sf () const;


    void loadQGISPlugins();
    void loadQGISTranslator();
    const QString & GISHomeDir() const;

    KKSCmdJSettings * getCmdJSettings();
    KKSMsgJSettings * getMsgJSettings();

    static KKSApplication * instance() { return self; }
    static KKSApplication * init (int argc, 
                                  char *argv[],
                                  bool with_connect = true, 
                                  const QString & userName = QString(), 
                                  bool msgToWindow = true);
    //static int GUIConnect(QWidget * parent = NULL);


private:
    mutable KKSLoader * m_loader;
    mutable KKSPluginLoader * m_pluginLoader;
    mutable KKSFileLoader * m_fileLoader;
    mutable KKSPPFactory * m_ppf;
    mutable KKSEIOFactory * m_eiof;
    mutable KKSObjEditorFactory * m_objf;
    mutable KKSRubricFactory * m_rf;
    mutable KKSCatEditorFactory * m_catf;
    mutable KKSTemplateEditorFactory * m_tf;
    mutable KKSStuffFactory * m_sf;
    mutable KKSAttributesFactory * m_attrf;

    mutable QString m_GISHomeDir; //каталог для временного хранения выгруженных на клиент файлов проектов и слоев QGIS

    static KKSApplication * self;

    KKSCmdJSettings * m_savedCmdJSettings;
    KKSMsgJSettings * m_savedMsgJSettings;

    void initGUIFactories();

    void loadCmdJSetings();
    void loadMsgJSetings();

private slots:

    void saveCmdJSettings(KKSCmdJSettings * settings);
    void saveMsgJSettings(KKSMsgJSettings * settings);


};

#define kksApp (static_cast<KKSApplication *>(KKSApplication::instance()))

#endif
