#ifndef AZDIALCALCROUTE_H
#define AZDIALCALCROUTE_H

#include <QDialog>
#include <QComboBox>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>

namespace Ui {
class AzDialCalcRoute;
}

class AzDialCalcRoute : public QDialog
{
    Q_OBJECT
    
public:
    explicit AzDialCalcRoute(QSettings *linkSettings, QWidget *parent = 0);
    QSettings *linkSettings;
    ~AzDialCalcRoute();
    QComboBox * mComboLayerAoI;
    QComboBox * mComboLayerForest;
    QComboBox * mComboLayerWater;
    QComboBox * mComboLayerRelief;
    QComboBox * mComboColDirectionSlope;
    QComboBox * mComboColSlope;
    struct STRUCTcoordsRoute
    {
        double xStart;
        double yStart;
        double xFinish;
        double yFinish;
    };
    STRUCTcoordsRoute mCoordsRoute;
    QProgressBar * mProgressBar;
    QLabel * mProgressBarText;
    QString mStrStart;
    QString mStrFinish;
    void setFinish(bool bReturn = true);

private slots:
    void SLOTbuttonOkClick();
    void SLOTcloseDialog();
    void SLOTchangeTextStart(QString pStart);
    void SLOTchangeTextFinish(QString pFinish);
signals:
    void SIGNALbuttonsClick(bool);

private:
    Ui::AzDialCalcRoute *ui;
};

#endif // AZDIALCALCROUTE_H
