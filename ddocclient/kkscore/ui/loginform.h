//
//
// C++ Interface: $MODULE$
//
// Description:
//
//
// Author: ,,, <oss@shaman>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LoginForm_H
#define LoginForm_H

#include <QWidget>
#include <QDialog>

#include <KKSList.h>
#include "kksacclevel.h"

namespace Ui{
    class login_ext_form;
	class login_form;
};

class LoginExtForm : public QWidget//, public Ui::login_ext_form
{
  Q_OBJECT
  public:
    LoginExtForm(QWidget *parent = 0);
    virtual ~LoginExtForm();
private:
	friend class LoginForm;
	Ui::login_ext_form * ui;
};

class LoginForm : public QDialog//, private Ui::login_form
{
  Q_OBJECT
  public:
    LoginForm(const KKSList<KKSAccLevel*> & levels, KKSAccLevel & currentLevel, QWidget *parent = 0);
    virtual ~LoginForm();

    void setHost(const QString &str);
    void setName(const QString &str);
    void setUser(const QString &str, bool canEdit = true);
    void setPass(const QString &str);
    void setPort(const QString &str);
    //void setLevel(const QString &str);
    //void setPorts(const QMap<QString, QString> & ports);

    QString host() const;
    QString name() const;
    QString user() const;
    QString pass() const;
    QString port() const;
    QString level() const;
    int levelIndex() const;

  public slots:
      int exec (void);

  private:
    bool moreShown;

    LoginExtForm * ext;
	Ui::login_form * ui;

    QString sHost;
    QString sName;
    QString sPort;

    //QMap<QString, QString> m_ports;
    KKSList<KKSAccLevel*> m_levels;

  private slots:
    virtual void pbOptions_clicked();
    void on_cbLevel_currentIndexChanged(const QString& level);
};

#endif
