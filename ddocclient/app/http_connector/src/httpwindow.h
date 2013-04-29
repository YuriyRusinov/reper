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
#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QDialog>

//#ifndef WIN32
//#include "cryptmessage.h"
//#endif
#include <JKKSLoader.h>
#include <JKKSPMessage.h>
//gui
class QDialogButtonBox;
class QFile;
class QLabel;
class QLineEdit;
class QProgressDialog;
class QPushButton;
//net
class QHttp;
class QHttpResponseHeader;
class QTcpServer;
class QSslError;
class QAuthenticator;


class HttpWindow : public QDialog
{
    Q_OBJECT

public:
    HttpWindow(QWidget *parent = 0);
    bool sendOutMessage( const JKKSPMessWithAddr * message, bool filePartsFlag = true);//filePartsFlag - флаг передачи файла по частям
    bool setMessageAsSended(const int & id, const int & type);
    
private slots:
    void startProc();
    void startTimer();
    void loadData();
    void cancelDownload();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void enableDownloadButton();

    void slotHttpMessageRemoved(int progress);
   /* void slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void sslErrors(const QList<QSslError> &errors);
#endif
    */

signals:
    void httpMessageRemoved(int progress);

private:
    QLabel *statusLabel;
    QLabel *urlLabel;
    QLineEdit *urlLineEdit;
    QProgressDialog *progressDialog;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QList<JKKSPMessWithAddr *> messageList;

    QMap<int, QPair<int, int> > httpMessages;

    //QList< QPair<int, int> > http_message;
    //QList< QPair<int, int> > message_type;


    QHttp *http;
    JKKSLoader * loader;

    QTimer * m_timer;
    QString localDBInfo;
    
    QTcpServer *tcpServer;
    QStringList responses;

    int httpGetId;
    int user_timer;
    int count_send;
    bool httpRequestAborted;
    bool httpTransferComplete;
	bool manual;
};

#endif
