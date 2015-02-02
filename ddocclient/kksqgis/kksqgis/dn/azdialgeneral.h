#ifndef AZDIALGENERAL_H
#define AZDIALGENERAL_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
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
    QDoubleSpinBox * mDoubleSpinBox;
    bool mOkClick;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AzDialGeneral *ui;

};

#endif // AZDIALGENERAL_H
