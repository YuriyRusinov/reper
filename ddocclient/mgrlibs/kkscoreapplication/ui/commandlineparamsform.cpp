#include <QtCore>
#include <QtGui>

#include "ui_command_line_params_form.h"

#include "commandlineparamsform.h"
#include "kkscommandlineopts.h"


CommandLineParamsForm:: CommandLineParamsForm(QWidget * parent)
    : QDialog (parent), UI (new Ui::command_line_params_form)
{
    UI->setupUi (this);

    UI->teHelp->setPlainText(KKSCommandLineOpts::getHelpMessage());

    connect (UI->pbClose, SIGNAL (clicked ()), this, SLOT (pbCloseClicked ()) );
}

CommandLineParamsForm:: ~CommandLineParamsForm( )
{
}

void CommandLineParamsForm:: pbCloseClicked ( void )
{
    accept ();
}


