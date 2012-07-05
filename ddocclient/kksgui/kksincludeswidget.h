#ifndef KKSINCLUDESWIDGET_H
#define KKSINCLUDESWIDGET_H

#include <QWidget>
#include <kksgui_config.h>

#include <KKSList.h>
#include <QModelIndex>
#include <QItemDelegate>
#include <QItemSelection>


class QTreeView;
class QGroupBox;
class QToolBar;
class QSplitter;
class QAction;

class KKSRubric;
class KKSRubricItem;
class RubricForm;
class KKSRecWidget;

class _GUI_EXPORT KKSIncludesWidget: public QWidget
{
    Q_OBJECT

public:
    KKSIncludesWidget (KKSRubric * rootRubric,
                       bool isAttach=true,
                       bool isDocs=false,
                       bool forCategory=false,
                       bool forRecord=true,
                       QWidget *parent = 0,
                       Qt::WindowFlags flags = 0);

    ~KKSIncludesWidget();

    KKSRubric * rootRubric() const;

    void hideIncludes (void);
    void showIncludes (void);

    QTreeView *tvRubr (void);

    void save (void);
    void setSaved (bool isSaved);

protected:
    //
    // Overrides
    //
    virtual void closeEvent (QCloseEvent * event);

private slots:
    //void on_pbOK_clicked();
    //void on_pbCancel_clicked();
    void addRubric (void);
    void editRubric (void);
    void delRubric (void);
    void copyFromRubric (void);

    void addRubricItem (void);
    void editRubricItem (void);
    void slotRubricItemDblClicked(const QModelIndex & index);
    void delRubricItem (void);

    void addSearchTemplateIntoRubric (void);
    void addCategoryIntoRubric (void);
    void rubricSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected);

public slots:
    void slotAddRubricItem(int idRubricItem, QString name);

private slots:
    void init();

signals:
    void rubricItemRequested();
    void openRubricItemRequested(int idRubricItem);
    void saveRubric (KKSRubric * r, bool isMyDocs);
    void rubricsChanged (void);
    void copyFromRubr (KKSRubric * rDest, QAbstractItemModel * attachModel, const QModelIndex& index);

    void aboutToClose();

    void loadStuffModel (RubricForm * rForm);
    void loadSearchtemplate (RubricForm * rForm);
    void loadCategory (RubricForm * rForm);
    void rubricAttachmentsView (QAbstractItemModel * attachModel, const KKSRubric * r);

private:
    //
    // Variables
    //
    //Ui::kksincludes_widget * ui;
    KKSRubric * m_rootRubric;
    bool isMyDoc;
    bool isChanged;
    bool isRec;
    QToolBar * tBRubrActions;
    QSplitter * spRubrics;
    QTreeView * twIncludes;
    QTreeView * tvItems;
    KKSRecWidget * recWItems;

private:
    //
    // Functions
    //
    void initActions (void);
    KKSRubric * currentRubric();
    KKSRubric * getRubric(QModelIndex index);

    QModelIndex appendRubricRow(const KKSRubric * r, QModelIndex index);
    QModelIndex appendItemRow(const KKSRubricItem * item, QModelIndex index);
    void initTwIncludes();
    void parseRubric(KKSRubric * r, QModelIndex index);
    void parseItems(KKSRubric * r, QModelIndex index);
};


class KKSIncludesItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    KKSIncludesItemDelegate(QObject * parent = 0);
    virtual ~KKSIncludesItemDelegate ();

    //void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    //QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // KKSFILEWIDGET_H
