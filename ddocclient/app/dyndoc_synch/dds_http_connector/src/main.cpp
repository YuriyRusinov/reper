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

#include "httpwindow.h"
#include "transportsettingsform.h"
#include "kkssito.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KKSSito *sito = KKSSito::init (false);
    if(!sito)
        return 1;

    QSettings settings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);
        //("/etc/kkssito/http_client.ini1", QSettings::IniFormat);

    dds_TransportSettingsForm * sForm = new dds_TransportSettingsForm (&settings);
    if (!sForm)
        return 1;
    if (sForm->exec () != QDialog::Accepted)
    {
        delete sForm;
        return 1;
    }
    settings.sync ();
    delete sForm;

    dds_HttpWindow httpWin;
    httpWin.show();
    return httpWin.exec();
}
