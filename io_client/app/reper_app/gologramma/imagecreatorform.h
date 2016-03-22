#ifndef IMAGECREATORFORM_H
#define IMAGECREATORFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMap>

#include "gologramma.h"

namespace Ui {
class imageCreatorForm;
}

class _GOL_EXPORT imageCreatorForm : public QDialog
{
    Q_OBJECT

public:
    explicit imageCreatorForm(QWidget *parent = 0, Qt::WindowFlags flags=0);
    ~imageCreatorForm();

    bool forTests (void) const;
    void initShipTypes (const QMap<int, QString>& shipTypes);

    int getShipType (void) const;
    double getResolution (void) const;

private:
    Ui::imageCreatorForm *ui;

    void formInit();
    int controllSpinBox();

private slots:
    void selectFileName();
    void build();

signals:
    void imagesData(generatingDataPlus);
};

#endif // IMAGECREATORFORM_H
