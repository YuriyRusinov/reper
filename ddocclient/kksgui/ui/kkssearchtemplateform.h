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
    class kkssearch_template_form;
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

class _GUI_EXPORT KKSSearchTemplateForm : public QDialog
{
    Q_OBJECT

public:
    KKSSearchTemplateForm (const KKSCategory * _c, 
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
    virtual ~KKSSearchTemplateForm();

    KKSList<const KKSFilterGroup*> & filters();
    void setFilters (const KKSList<const KKSFilterGroup*> & filters);

    KKSSearchTemplate * searchT (void) const;
    void setSearchTemplate (KKSSearchTemplate * st);

private slots:
    void viewSQL();
    
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
    void saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory * c); //c - ��������� ��� ������� ����� ��������� �������� ������
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
    Ui::kkssearch_template_form * ui;
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

    QString m_parentTable;//������������ ��� ��������� ��������/�������. 
                          //��������� �� ����� ������� �� ����� ������ �������� �������, 
                          //�� ������� ������ �������, �� ��� �������� ���������� �������� � ������ ����� �����
};

#endif // KKSFILTERSEDITORFORM_H
