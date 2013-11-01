#ifndef NETTHREAD_H
#define NETTHREAD_H

#include <QThread>
#include <QMessageBox>
#include <QSettings>
#include <QPushButton>
#include <QHBoxLayout>

#include "httpwindow.h"

#include "../dyndoc_synch/Namespaces/netStruct.h"

dyndoc_mainStructs::netThreadInf translateSettings(QSettings &settings);

class netThread : public QThread
{
    Q_OBJECT

public:
    netThread(const dyndoc_mainStructs::netThreadInf &rhs, QObject* parent = 0);
    ~netThread();

signals:


public slots:
    void slotTest();

private:
    dyndoc_mainStructs::netThreadInf data;

    void run();
};

class testForm: public QDialog
{
    Q_OBJECT

public:
    testForm(const dyndoc_mainStructs::netThreadInf &rhs, QWidget *parent = 0);
    ~testForm();

private:
    dyndoc_mainStructs::netThreadInf data;

    netThread* net;

signals:
    void okCl();
    void clCl();

public slots:
    void openThread();

    void ok_clicked();
    void close_clicked();
};

#endif
