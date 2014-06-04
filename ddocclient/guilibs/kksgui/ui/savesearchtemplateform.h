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
class QSortFilterProxyModel;
class QModelIndex;
class QItemSelection;

class KKSSearchTemplate;

class _GUI_EXPORT SaveSearchTemplateForm : public QDialog
{
public:
    SaveSearchTemplateForm (KKSSearchTemplate * st, bool mode=true, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SaveSearchTemplateForm (void);

    void setCategoryModel (QAbstractItemModel * catMod);
    void setTypesModel (QAbstractItemModel * typeMod);
    int getIdCat (void) const;
    int getIdType (void) const;
    QString getName (void) const;
    
    KKSSearchTemplate * getSearchTemplate (void) const;
    void selectType (const QModelIndex& tInd) const;
    void selectCategory (const QModelIndex& cInd) const;

private slots:
    void staccept (void);
    void catChanged (const QModelIndex& current, const QModelIndex& previous);
    void selCatChanged (const QItemSelection& selected, const QItemSelection& deselected);
    
signals:
    void categoryChanged (KKSSearchTemplate * st, int idCat);

private:
    //
    // Functions
    //
    void init (void);

private:
    //
    // Variables
    //
    Ui::save_search_template_form * UI;
    KKSSearchTemplate * searchTemplate;
    QSortFilterProxyModel * sortTModel;

private:
    Q_OBJECT
};

#endif
