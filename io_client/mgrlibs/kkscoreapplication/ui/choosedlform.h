#ifndef _CHOOSEDL_FORM_H
#define _CHOOSEDL_FORM_H

#include <QDialog>
#include <QString>

#include "ui_choose_dl_form.h"


class ChooseDlForm : public QDialog, private Ui::choose_dl_form
{
    public:
        ChooseDlForm (QWidget * parent=NULL);
        virtual ~ChooseDlForm ( );

        int currentDl ( void ) const;
        void addDl(int id, const QString & dlName, const QString & posName, const QString & unitName);

    public slots:
        void pbOKClicked();
        void pbCancelClicked();
        void dblClickedSlot(const QModelIndex & );


    private:

        Q_OBJECT
};

#endif
