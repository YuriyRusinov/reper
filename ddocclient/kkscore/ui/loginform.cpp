#include <QPushButton>
#include <QLineEdit>
#include <QtDebug>

#include "loginform.h"
#include "ui_login_form.h"
#include "ui_login_ext_form.h"

LoginExtForm::LoginExtForm(QWidget *parent) : QWidget(parent)
{
	ui = new Ui::login_ext_form;
	ui->setupUi(this);
}
LoginExtForm::~LoginExtForm()
{
}

LoginForm::LoginForm(const KKSList<KKSAccLevel*> & levels, KKSAccLevel & currentLevel, QWidget *parent) : QDialog(parent)
{
	ui = new Ui::login_form;
	ui->setupUi(this);

    ext = NULL;

    connect( ui->pbOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( ui->pbCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( ui->pbOptions, SIGNAL( clicked() ), this, SLOT( pbOptions_clicked() ) );

    m_levels = levels;
    int cnt = m_levels.count();
    bool bIndexSet = false;
    for(int i=0; i<cnt; i++){
        QString levelName = m_levels.at(i)->name();
        QString levelPort = m_levels.at(i)->port();
        ui->cbLevel->addItem(levelName, levelPort);
        if(currentLevel.name() == levelName){
            ui->cbLevel->setCurrentIndex(i);
            bIndexSet = true;
        }
    }
    if(!bIndexSet && m_levels.count() > 0)
        ui->cbLevel->setCurrentIndex(0);
    
    //connect( ui->cbLevel, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(on_cbLevel_itemChanged(const QString&)));

    moreShown = false;

    setOrientation(Qt::Vertical);
}

LoginForm::~LoginForm()
{
}

void LoginForm::pbOptions_clicked()
{
    if ( ext == 0 )
    {
        ext = new LoginExtForm( this );
        setExtension( ext );
        ext->ui->leDBName->setText( sName );
        ext->ui->leDBServer->setText( sHost );
        ext->ui->leDBPort->setText( sPort );
    }

    moreShown = !moreShown;
    showExtension(moreShown);
    QString txt = moreShown ? "<<" : ">>";
    QString old = ui->pbOptions->text();
    old.truncate(old.length() - 2 );
    ui->pbOptions->setText(old + txt);
}

void LoginForm::setHost( const QString &str )
{
    sHost = str;
}

void LoginForm::setName( const QString &str )
{
    sName = str;
}

void LoginForm::setPort( const QString &str )
{
    sPort = str;
}

void LoginForm::setUser( const QString &str, bool canEdit)
{
    ui->leUserName->setText( str );
	
	ui->leUserName->setEnabled(canEdit);
}

void LoginForm::setPass( const QString &str )
{
    ui->lePassword->setText( str );
}

QString LoginForm::host( ) const
{
    return ext ? ext->ui->leDBServer->text() : sHost;
}

QString LoginForm::port( ) const
{
    return ext ? ext->ui->leDBPort->text() : sPort;
}

QString LoginForm::name( ) const
{
    return ext ? ext->ui->leDBName->text() : sName;
}

QString LoginForm::user( ) const
{
    return ui->leUserName->text();
}

QString LoginForm::pass( ) const
{
    return ui->lePassword->text();
}

void LoginForm::on_cbLevel_currentIndexChanged(const QString& level)
{
    int index = ui->cbLevel->findText(level);
    if(index < 0)
        return;

    QString port = ui->cbLevel->itemData(index).toString();

    setPort(port);
    if(ext)
        ext->ui->leDBPort->setText(port);
}

QString LoginForm::level() const
{
    return ui->cbLevel->currentText();
}

int LoginForm::levelIndex() const
{
    return ui->cbLevel->currentIndex();
}

int LoginForm::exec (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    int r = QDialog::exec();
    return r;
}
