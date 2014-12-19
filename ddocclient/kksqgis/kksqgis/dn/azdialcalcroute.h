#ifndef AZDIALCALCROUTE_H
#define AZDIALCALCROUTE_H

#include <QDialog>

namespace Ui {
class AzDialCalcRoute;
}

class AzDialCalcRoute : public QDialog
{
    Q_OBJECT
    
public:
    explicit AzDialCalcRoute(QWidget *parent = 0);
    ~AzDialCalcRoute();
    
private:
    Ui::AzDialCalcRoute *ui;
};

#endif // AZDIALCALCROUTE_H
