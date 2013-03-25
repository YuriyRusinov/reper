#ifndef _SaveSearchTemplateForm_H
#define _SaveSearchTemplateForm_H

#include <QDialog>
#include <QModelIndex>

#include <kksgui_config.h>

namespace Ui
{
    class save_search_template_form;
};

class QAbstractItemModel;

class KKSSearchTemplate;

class _GUI_EXPORT SaveSearchTemplateForm : public QDialog
{
public:
    SaveSearchTemplateForm (KKSSearchTemplate * st, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SaveSearchTemplateForm (void);

    void setCategoryModel (QAbstractItemModel * catMod);
    void setTypesModel (QAbstractItemModel * typeMod);
    int getIdCat (void) const;
    int getIdType (void) const;
    QString getName (void) const;
    
    KKSSearchTemplate * getSearchTemplate (void) const;

private slots:
    void staccept (void);

private:
    //
    // Functions
    //
    void init (void);
    const QModelIndex& getCurrentType (int idType, const QModelIndex& pIndex=QModelIndex()) const;

private:
    //
    // Variables
    //
    Ui::save_search_template_form * UI;
    KKSSearchTemplate * searchTemplate;

private:
    Q_OBJECT
};

#endif
