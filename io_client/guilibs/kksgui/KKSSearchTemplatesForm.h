#ifndef _KKSSearchTemplatesForm_H
#define _KKSSearchTemplatesForm_H

#include <KKSRecDialog.h>
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
class QCheckBox;

class KKSCategory;

class _GUI_EXPORT KKSSearchTemplatesForm : public KKSRecDialog
{
public:
    KKSSearchTemplatesForm (const KKSCategory * c, const QString & tableName, bool mode=true, QWidget * parent=0, Qt::WFlags f=0);
    ~KKSSearchTemplatesForm (void);

    virtual qint64 getID (void) const;
    int getIdSearchTemplate (void) const;
    int getIdSearchTemplateType (void) const;
    void hideActions (int aBegin, int aEnd);
    void showActions (int aBegin, int aEnd);
    QAbstractItemModel *dataModel (void);
    void setDataModel (QAbstractItemModel * mod);
    void setUserId (int iduser);
    QItemSelectionModel * selectionModel (void) const;
    
    int getIndicatorSection (void) const;
    Qt::SortOrder getIndicatorOrder (void) const;
    
    Qt::CheckState getSearchInRes (void) const;
    
    void setResultsVisible (bool isVisible);

private slots:
    void addEmptySearchTemplate (void);
    void addCopySearchTemplate (void);
    void editSearchTemplate (void);
    void delSearchTemplate (void);
    
    void addSearchTemplateType (void);
    void editSearchTemplateType (void);
    void delSearchTemplateType (void);

    void searchTemplatesSelectionChanged (const QItemSelection & selected, const QItemSelection & deselected);
    
    void refreshSearchTemplates (void);

    void setFilterSt (const QString& text);
    
    void executeSt (void);

signals:
    void addNewEmptySearchTemplate (const QModelIndex& parent, 
                                    QAbstractItemModel * searchMod, 
                                    const KKSCategory * c,//c - ���������, ��� ������� ����� ��������� ������ ��������� ������
                                    const QString & tableName);
    void addNewCopySearchTemplate (const QModelIndex& wIndex, QAbstractItemModel * searchMod); 
    void updateSearchTemplate (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    void deleteSearchTemplate (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    
    void addSearchTemplateType (const QModelIndex& parent, QAbstractItemModel * searchMod);
    void updateSearchTemplateType (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    void delSearchTemplateType (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    
    void refSearchTemplates (QAbstractItemModel * searchMod);
    
    void applySearchTemplate (int);

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

    const KKSCategory * c; //��������� � ������� ������ ���� �������� 
                           //���������� ��� ����������� � ������ ����� ��������� ������
    QString m_tableName; //������������ ������ ��� �������� � ����� �������� ���������� �������
                         //����� ��� ��������� ��������� ���� atCheckListEx
                           
    QTreeView * searchView;
    QToolBar * tbActions;
    QAction * actAddNew;
    QAction * actAddCopy;
    QAction * actEdit;
    QAction * actDel;
    
    QAction * actAddNewType;
    QAction * actEditType;
    QAction * actDelType;
    
    QAction * actRefresh;
    
    QAction * actExecute;

    QLineEdit * lEFilter;

    QCheckBox * chSearchInResults;
    QPushButton * pbOk;
    QPushButton * pbCancel;

private:
    Q_OBJECT
};

#endif
