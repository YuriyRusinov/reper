#ifndef AZDIALCALCROUTE_H
#define AZDIALCALCROUTE_H

#include <QDialog>
#include <QComboBox>
#include <QProgressBar>
#include <QLabel>

namespace Ui {
class AzDialCalcRoute;
}

class AzDialCalcRoute : public QDialog
{
    Q_OBJECT
    
public:
    explicit AzDialCalcRoute(QWidget *parent = 0);
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

private slots:
    void SLOTbuttonOkClick();
    void SLOTcloseDialog();

signals:
    void SIGNALbuttonsClick(bool);

private:
    Ui::AzDialCalcRoute *ui;
};

#endif // AZDIALCALCROUTE_H
