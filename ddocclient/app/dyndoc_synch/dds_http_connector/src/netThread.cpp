#include "netThread.h"

dyndoc_mainStructs::netThreadInf translateSettings(QSettings& settings)
{
    dyndoc_mainStructs::netThreadInf data;

    settings.beginGroup ("Database");
    data.dataBase.dbName = settings.value ("database").toString ();
    data.dataBase.userName = settings.value ("user", "jupiter").toString ();
    data.dataBase.password = settings.value("password", "111").toString ();

    data.dataBase.hostAddress = settings.value ("host", "127.0.0.1").toString ();
    data.dataBase.port = settings.value("port", "5432").toInt ();
    settings.endGroup ();

    settings.beginGroup("Transport");
    data.transport.transportProtocol = settings.value ("transport","1").toInt ();
    settings.endGroup ();

    settings.beginGroup ("Http");

    data.transport.serverport = settings.value ("server_port", "8080").toInt ();

    data.transport.gateway.address = settings.value ("host", "").toString();
    data.transport.gateway.port = settings.value ("port", "0").toInt();

    settings.endGroup ();

    data.timer.timerFlag = false;
    data.timer.delay = 0;

    return data;
}

testForm::testForm(const dyndoc_mainStructs::netThreadInf &rhs, QWidget* parent):
    QDialog(parent),net(0)
{
    QPushButton* ok = new QPushButton("Ok",this);
    QPushButton* close = new QPushButton("close",this);

    connect(ok,SIGNAL(clicked()),this,SLOT(ok_clicked()));
    connect(close,SIGNAL(clicked()),this,SLOT(close_clicked()));

    connect(this,SIGNAL(okCl()),this,SLOT(openThread()));

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(ok);
    layout->addWidget(close);
    this->setLayout(layout);

    data = rhs;
    data.dataBase.hostAddress = "192.168.17.118";
}

testForm::~testForm()
{

}

void testForm::openThread()
{
    net = new netThread(data,this);

    connect(this,SIGNAL(clCl()),net,SLOT(quit()));

    net->start();
}

void testForm::ok_clicked()
{
    emit okCl();
}

void testForm::close_clicked()
{
    emit clCl();
}

netThread::netThread(const dyndoc_mainStructs::netThreadInf& rhs,QObject* parent):
    QThread(parent)
{    
    data = rhs;
}

netThread::~netThread()
{

}

void netThread::run()
{      
    dds_HttpWindow* httpWin = new dds_HttpWindow(data);

    exec();
}

void netThread::slotTest()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("param"));
    msgBox.exec();
}
