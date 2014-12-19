#ifndef IMAGECREATORFORM_H
#define IMAGECREATORFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "gologramma.h"

namespace Ui {
class image_creator_form;
}

class _GOL_EXPORT imageCreatorForm : public QDialog
{
    Q_OBJECT

public:
    explicit imageCreatorForm(QWidget *parent = 0);
    ~imageCreatorForm();

private:
    Ui::image_creator_form *ui;
    int controllSpinBox();

    void formInit();

private slots:
    void selectFileName();
    void build();

signals:
    void imagesData(generatingDataPlus);
};

#endif // IMAGECREATORFORM_H
