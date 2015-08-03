#ifndef _COMMANDLINEPARAMS_FORM_H
#define _COMMANDLINEPARAMS_FORM_H

#include <QDialog>
#include <QString>

namespace Ui
{
    class command_line_params_form;
};

class CommandLineParamsForm : public QDialog //, private Ui::command_line_params_form
{
    public:
        CommandLineParamsForm(QWidget * parent=NULL);
        virtual ~CommandLineParamsForm ( );

    public slots:
        void pbCloseClicked();

    private:
        Ui::command_line_params_form * UI;
        
        Q_OBJECT
};

#endif
