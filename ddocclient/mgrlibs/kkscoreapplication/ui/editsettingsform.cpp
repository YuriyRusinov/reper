#include <QtCore>
#include <QtGui>

#include "editsettingsform.h"

EditSettingsForm :: EditSettingsForm (QString text, QWidget * parent )
    : QDialog (parent)
{
    setupUi (this);

    this->lineEdit->setText (text);
}

EditSettingsForm :: ~EditSettingsForm ( void )
{
}
