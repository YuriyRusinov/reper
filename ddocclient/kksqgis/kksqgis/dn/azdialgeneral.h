#ifndef AZDIALGENERAL_H
#define AZDIALGENERAL_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>

namespace Ui {
class AzDialGeneral;
}

class AzDialGeneral : public QDialog
{
    Q_OBJECT
    
public:
    explicit AzDialGeneral(QWidget *parent = 0, int typeInterface = 0);
    ~AzDialGeneral();
    QComboBox * mComboBoxOne;
    QLabel * mTextLabelOne;

private:
    Ui::AzDialGeneral *ui;
};

#endif // AZDIALGENERAL_H
