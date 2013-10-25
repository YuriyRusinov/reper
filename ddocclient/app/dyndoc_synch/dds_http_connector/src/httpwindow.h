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

#include <QtGui>
#include <QtNetwork>
#include <QDialog>

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

class netThread;

namespace dyndoc_HTTPconnector
{
    class HTTPconnectorError
    {
    public:
        HTTPconnectorError();
        virtual ~HTTPconnectorError();

        virtual QString getError() const;
    };

    class TCPserverError: public HTTPconnectorError
    {
    public:
        TCPserverError();
        virtual ~TCPserverError();

        virtual QString getError() const;
    };

    class loaderError: public HTTPconnectorError
    {
    public:
        loaderError();
        virtual ~loaderError();

        virtual QString getError() const;
    };

    struct HTTPsettings
    {
        QString dbName;
        QString host;
        QString user;
        QString password;
        int port;

        QString http_host;
        int http_port;
        int transport;
        QString addr;
        int server_port;
    };
}

class dds_HttpWindow : public QObject
{
    Q_OBJECT

public:
    dds_HttpWindow(dyndoc_HTTPconnector::HTTPsettings& settings, QObject *parent = 0);
    bool sendOutMessage( const JKKSPMessWithAddr * message, bool filePartsFlag = true);//filePartsFlag - флаг передачи файла по частям
    bool setMessageAsSended(const int & id, const int & type);

    void run();
    
private slots:
    void startProc();
    void loadData();
    void cancelDownload();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);

signals:
    void httpMessageRemoved(int progress);

    void signalCancelDownload();
    void signalErrorDataTransferFailed(QString str);
    void signalDataTransferComplete();

private:
    QList<JKKSPMessWithAddr *> messageList;

    QMap<int, QPair<qint64, qint64> > httpMessages;

    QString dbName;
    QString host;
    QString user;
    QString password;
    int port;

    QString http_host;
    int http_port;
    int transport;
    QString addr;
    int server_port;

    QHttp *http;
    JKKSLoader * loader;

    QString  localDBInfo;
    
    QTcpServer *tcpServer;
    QStringList responses;

    int  httpGetId;
    int  count_send;
    bool httpRequestAborted;
    bool httpTransferComplete;

    inline void init(dyndoc_HTTPconnector::HTTPsettings& settings);

    inline void init_interface();
    inline void init_TCPserver();
    inline void init_loader();
    inline void init_connections();

    inline void init_settings(dyndoc_HTTPconnector::HTTPsettings& settings);
    inline void init_settingsDB(dyndoc_HTTPconnector::HTTPsettings& settings);
    inline void init_settingsTransportProtocol(dyndoc_HTTPconnector::HTTPsettings& settings);
    inline void init_settingsServerAndLocal(dyndoc_HTTPconnector::HTTPsettings& settings);
};

#endif
