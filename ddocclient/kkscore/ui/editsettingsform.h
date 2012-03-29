#ifndef _EditSettingsForm_H
#define _EditSettingsForm_H

#include "ui_edit_settings_form.h"

class EditSettingsForm : public QDialog, private Ui::EditSettingsForm
{
    public:
        EditSettingsForm (QString text = QString (), QWidget * parent = NULL);
        ~EditSettingsForm ( void );

        QString getText ( void ) const { return this->lineEdit->text (); }
    private:
        Q_OBJECT
};

#endif
