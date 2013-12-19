#ifndef KKSFILTERSEDITORFORM_H
#define KKSFILTERSEDITORFORM_H

#include <QDialog>
#include <kksgui_config.h>

#include <KKSList.h>
#include <KKSMap.h>

#include <KKSAttribute.h>
#include <KKSFilter.h>
#include <KKSAttrType.h>

#include <QDate>
#include <QModelIndex>

namespace Ui
{
    class filters_editor_form;
}

class QLabel;
class QComboBox;
class QWidget;
class QPushButton;
class QLineEdit;
class QTextEdit;
class QCheckBox;
class QDateEdit;
class QDateTimeEdit;
class QListView;
class QAbstractItemModel;
class QSortFilterProxyModel;
class QStackedLayout;
class QToolButton;
class QAction;

class KKSCategory;
class KKSAttribute;
class KKSImage;
class KKSSearchTemplate;

class _GUI_EXPORT KKSFiltersEditorForm : public QDialog
{
    Q_OBJECT

public:
    KKSFiltersEditorForm(const KKSCategory * _c,
                         const QString & tableName,
#ifdef Q_CC_MSVC
                         KKSMap<int, KKSAttribute *> attrsIO = KKSMap<int, KKSAttribute*>(),
#else
                         KKSMap<int, KKSAttribute *> attrsIO,
#endif
                         bool forIO = false, 
                         QWidget *parent = 0,
                         Qt::WFlags f=0);

    KKSFiltersEditorForm (const KKSCategory * _c, 
                         const QString & tableName,
#ifdef Q_CC_MSVC
                         KKSMap<int, KKSAttribute *> attrsIO = KKSMap<int, KKSAttribute*>(),
#else
                         KKSMap<int, KKSAttribute *> attrsIO,
#endif
                         bool forIO = false,
                         KKSSearchTemplate * st=0,
                         bool mode = true,
                         QWidget *parent = 0,
                         Qt::WFlags f=0);
    virtual ~KKSFiltersEditorForm();

    KKSList<const KKSFilterGroup*> & filters();
    void setFilters (const KKSList<const KKSFilterGroup*> & filters);

    KKSSearchTemplate * searchT (void) const;
    void setSearchTemplate (KKSSearchTemplate * st);

private slots:
    void on_pbShowSQL_clicked();
    
    void addGroup();
    void attrChanged (int index);
    void addFilter (void);
    void setValueWidget (int index);

    void setBoolValChanged (int state);
    void loadImage (void);

    void saveSQLQuery (void);
    void loadSQLQuery (void);

    void saveSQLAccept (void);
    void delFilter (void);

signals:
    //void loadAttributeRefValues (const QString & tableName, const KKSAttribute * attr, QComboBox * cbList);
    void loadAttributeRefValues (const QString & tableName, const KKSAttribute * attr, QAbstractItemModel * mod);
    void saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory * c); //c - категория для которой можно применить поисковы запрос
    void loadSearchCriteria (void);//QAbstractItemModel *);

private:
    //
    // Functions
    //
    void init();
    void initFilterTypes (KKSAttrType::KKSAttrTypes type);
    void initAttrs();
    void initValuesWidgets (void);
    void createGroup (bool AND = true);
    void updateSQL ();

    void setFiltersModel (QAbstractItemModel * mod, const QModelIndex& parent, const KKSFilterGroup * parentGroup);

private:
    //
    // Variables
    //
    Ui::filters_editor_form * ui;
    QLabel * lAttribute;
    QLabel * lOper;
    QLabel * lValue;
    QComboBox * cbAttribute;
    QComboBox * cbOper;

    QWidget * wValue;
    QStackedLayout * stLayValue;

    QLineEdit * lEValue;
    QLineEdit * lEStrValue;
    QCheckBox * chCaseSensitive;
    //QComboBox * cbValue;
    QTextEdit * teValue;
    QCheckBox * chTextCaseSensitive;
    QCheckBox * chValue;
    QDateEdit * dValue;
    QDateTimeEdit * dtValue;
    QLineEdit * lEIntervalValue;
    QComboBox * cbUnits;
    QListView * lvCheckRef;
    QSortFilterProxyModel * sortRefModel;
    QAbstractItemModel * checkRefModel;
    KKSImage * wImage;
    QToolButton * tbImage;

    QPushButton * pbAddFilter;

    const KKSCategory * c;
    KKSList<const KKSFilterGroup*> m_filters;
    
    KKSMap<int, KKSAttribute *> m_attrsIO;

    KKSMap<int, KKSAttribute *> m_attrsAll;
    bool m_bForIO;
    KKSSearchTemplate *sTempl;
    QAction * delSearchEntity;
    bool isDbSaved;

private:
    //
    // Functions
    //
    QString parseGroup(const KKSFilterGroup * g, const QString & tableName);
    QString parseFilter(const KKSFilter * f, const QString & tableName);
    KKSFilterGroup * currentGroup();
    KKSFilterGroup * getGroup(QModelIndex index);

    QString m_parentTable;//используется для атрибутов родитель/потомок. 
                          //Поскольку мы через атрибут не можем узнать название таблицы, 
                          //из которой делаем выборку, то это название необходимо передать в данную форму извне
};

#endif // KKSFILTERSEDITORFORM_H
