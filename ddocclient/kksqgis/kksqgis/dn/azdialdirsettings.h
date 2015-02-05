#ifndef AZDIALDIRSETTINGS_H
#define AZDIALDIRSETTINGS_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>

namespace Ui {
class AzDialDirSettings;
}

class AzDialDirSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit AzDialDirSettings(QSettings *linkSettings, QWidget *parent = 0);
    QSettings *linkSettings;
    ~AzDialDirSettings();
    QString fromSlashToBackslash(QString pStr);
    QString fromBackslashToSlash(QString pStr);


private slots:
    void SLOTpathOne();
    void SLOTpathTwo();
    void SLOTpathChange(int pVar = 1);

public slots:
    void SLOTaccepted();
    void SLOTrejected();

signals:
    void SIGNALchangeUi(int);

private:
    Ui::AzDialDirSettings *ui;
};

#endif // AZDIALDIRSETTINGS_H
