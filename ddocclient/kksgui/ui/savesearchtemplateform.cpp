#include "savesearchtemplateform.h"
#include "ui_save_search_template_form.h"

SaveSearchTemplateForm :: SaveSearchTemplateForm (QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::save_search_template_form)
{
    UI->setupUi (this);
}

SaveSearchTemplateForm :: ~SaveSearchTemplateForm (void)
{
    delete UI;
}
