#include "azdialdirsettings.h"
#include "ui_azdialdirsettings.h"

AzDialDirSettings::AzDialDirSettings(QSettings *linkSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AzDialDirSettings)
{
    ui->setupUi(this);
    this->setWindowTitle("Настройка каталогов");
    ui->pathPluginsQGIS->setReadOnly(true);
    ui->pathWorkDir->setReadOnly(true);
    QObject::connect(ui->pathChange, SIGNAL(clicked()), this, SLOT(SLOTpathOne()));
    QObject::connect(ui->pathChange2, SIGNAL(clicked()), this, SLOT(SLOTpathTwo()));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(SLOTaccepted()));
    QObject::connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(SLOTrejected()));
    QObject::connect(this, SIGNAL(SIGNALchangeUi(int)), this, SLOT(SLOTpathChange(int)));
    this->linkSettings = linkSettings;
    ui->pathWorkDir->setText(fromBackslashToSlash(linkSettings->value("MainSettings/SaveDirPath", "C:/").toString()));
    ui->pathPluginsQGIS->setText(fromBackslashToSlash(linkSettings->value("MainSettings/PluginsDirPath", "C:/REP_EXT/QGS22/plugins").toString()));
}

AzDialDirSettings::~AzDialDirSettings()
{
    delete ui;
}

QString AzDialDirSettings::fromSlashToBackslash(QString pStr)
{
    QString pStr1 = pStr;
    pStr1.replace("\\", "/");
    return pStr1;
}

QString AzDialDirSettings::fromBackslashToSlash(QString pStr)
{
    QString pStr1 = pStr;
    pStr1.replace("/", "\\");
    pStr1.replace("\\\\", "\\");
    return pStr1;
}

void AzDialDirSettings::SLOTaccepted()
{
    QString pSs = fromSlashToBackslash(ui->pathWorkDir->text());
    linkSettings->setValue("MainSettings/SaveDirPath", pSs);
    pSs = fromSlashToBackslash(ui->pathPluginsQGIS->text());
    linkSettings->setValue("MainSettings/PluginsDirPath", pSs);
    this->close();
}

void AzDialDirSettings::SLOTrejected()
{
    this->close();
}

void AzDialDirSettings::SLOTpathChange(int pVar)
{
    QString sDir("");
    QFileDialog DirChoice;
    sDir = DirChoice.getExistingDirectory(this,"Выберите каталог", sDir);
    if(!sDir.isEmpty())
    {
        if (sDir[sDir.size()-1]!='/' &&  sDir[sDir.size()-1]!='\\')
        {
            sDir = sDir + "/";
        }
        sDir = fromBackslashToSlash(sDir);
        switch (pVar)
        {
        case 1: // qgis dir
            ui->pathPluginsQGIS->setText(sDir);
            break;
        case 2: // work dir
            ui->pathWorkDir->setText(sDir);
            break;
        }
    }
}

void AzDialDirSettings::SLOTpathOne()
{
    emit SIGNALchangeUi(1); // plugins
}

void AzDialDirSettings::SLOTpathTwo()
{
    emit SIGNALchangeUi(2); // work dir
}
