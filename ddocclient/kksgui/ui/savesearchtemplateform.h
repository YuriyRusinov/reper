#ifndef _SaveSearchTemplateForm_H
#define _SaveSearchTemplateForm_H

#include <QDialog>
#include <kksgui_config.h>

namespace Ui
{
    class save_search_template_form;
};

class QAbstractItemModel;

class SaveSearchTemplateForm : public QDialog
{
public:
    SaveSearchTemplateForm (QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SaveSearchTemplateForm (void);

    void setCategoryModel (QAbstractItemModel * catMod);
    int getIdCat (void) const;
    QString getName (void) const;

private slots:
    void catChStateChanged (int state);

private:
    //
    // Variables
    //
    Ui::save_search_template_form * UI;

private:
    Q_OBJECT
};

#endif
