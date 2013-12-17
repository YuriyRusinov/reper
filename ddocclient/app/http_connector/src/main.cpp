/****************************************************************************
**
** Copyright (C) 2004-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QApplication>
#include <QSettings>
#include <QTextCodec>

#include <ddocinteractorwindow.h>
#include <ddocinteractorbase.h>
#include <timerform.h>
#include "transportsettingsform.h"
#include "kkssito.h"
#include "kksdebug.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));

    KKSDebug::setMinMsgType(KKSInfoMsg);
    KKSDebug::setUseQDebug(true);

    KKSSito *sito = KKSSito::init (argc, argv, false, QString(), false);
    if(!sito)
        return 1;

    ///////////
    //Init main settings
    ///////////

    QSettings settings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);

    TransportSettingsForm * sForm = new TransportSettingsForm (&settings);
    if (!sForm)
        return 1;
    if (sForm->exec () != QDialog::Accepted)
    {
        delete sForm;
        return 1;
    }
    settings.sync ();
    delete sForm;

    int res = 1;

    //////////
    //Init timer settings
    //////////

    int intervalMs = 10000;
    int interval = 10;
    int units = 0;//seconds
    bool mode = false; //true = manual

    TimerForm * timerForm = new TimerForm ();
    if (!timerForm){
        return 1;
    }

    timerForm->init(interval, units, mode, true);
    
    if (timerForm->exec () != QDialog::Accepted){
		delete timerForm;
        return 1;
    }
    else
    {
		mode = timerForm->startManually();
        intervalMs = timerForm->getTimerMs();
    }
    delete timerForm;

    ////////////
    //Start log window and threads
    ////////////

    DDocInteractorWindow httpWin;
    DDocInteractorBase * m_base = new DDocInteractorBase(NULL);
    
    httpWin.setTimerParams(interval, units, mode);
    httpWin.setInteractorBase(m_base);    

    int ok = m_base->start(mode, intervalMs);
    if(ok != 1)
        return 1;


    ///////////
    //Show log window
    //////////
    if(ok){
        httpWin.show();
    
        res = httpWin.exec();
    }

    return res;
}
