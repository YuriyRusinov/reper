#ifndef _KKSSearchTemplatesForm_H
#define _KKSSearchTemplatesForm_H

#include <QDialog>
#include <kksgui_config.h>

class QTreeView;
class QPushButton;
class QAbstractItemModel;
class QItemSelection;
class QItemSelectionModel;
class QModelIndex;
class QAction;
class QToolBar;
class QLineEdit;

class KKSCategory;

class _GUI_EXPORT KKSSearchTemplatesForm : public QDialog
{
public:
    KKSSearchTemplatesForm (const KKSCategory * c, const QString & tableName, QWidget * parent=0, Qt::WFlags f=0);
    ~KKSSearchTemplatesForm (void);

    int getIdSearchTemplate (void) const;
    QAbstractItemModel *dataModel (void);
    void setDataModel (QAbstractItemModel * mod);
    void setUserId (int iduser);
    QItemSelectionModel * selectionModel (void) const;

private slots:
    void addEmptySearchTemplate (void);
    void addCopySearchTemplate (void);
    void editSearchTemplate (void);
    void delSearchTemplate (void);
    
    void addSearchTemplateType (void);
    void editSearchTemplateType (void);
    void delSearchTemplateType (void);

    void searchTemplatesSelectionChanged (const QItemSelection & selected, const QItemSelection & deselected);

    void setFilterSt (const QString& text);

signals:
    void addNewEmptySearchTemplate (const QModelIndex& parent, 
                                    QAbstractItemModel * searchMod, 
                                    const KKSCategory * c,//c - категория, для которой можно приемнить данный поисковый запрос
                                    const QString & tableName);
    void addNewCopySearchTemplate (const QModelIndex& wIndex, QAbstractItemModel * searchMod); 
    void updateSearchTemplate (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    void deleteSearchTemplate (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    
    void addSearchTemplateType (const QModelIndex& parent, QAbstractItemModel * searchMod);
    void updateSearchTemplateType (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    void delSearchTemplateType (const QModelIndex& wIndex, QAbstractItemModel * searchMod);

private:
    //
    // Functions
    //
    void init (void);
    QModelIndex getCurrentIndex (void) const;

private:
    //
    // Variables
    //
    int idUser;

    const KKSCategory * c; //категория к которой должен быть применен 
                           //выбираемый или создаваемый в данной форме поисковый запрос
    QString m_tableName; //используется только для передачи в форму создания поискового запроса
                         //нужно для обработки атрибутов типа atCheckListEx
                           
    QTreeView * searchView;
    QToolBar * tbActions;
    QAction * actAddNew;
    QAction * actAddCopy;
    QAction * actEdit;
    QAction * actDel;
    
    QAction * actAddNewType;
    QAction * actEditType;
    QAction * actDelType;

    QLineEdit * lEFilter;

    QPushButton * pbOk;
    QPushButton * pbCancel;

private:
    Q_OBJECT
};

#endif
