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
class QContextMenuEvent;
class QMenu;
class QAbstractItemModel;

class KKSRubric;
class KKSRubricItem;
class KKSRubricBase;
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
    
    QAbstractItemModel * rubrModel () const;
    void setRubrModel (QAbstractItemModel * rModel);
    
    void addRubricIntoModel (KKSRubric * cRubr, const QModelIndex& pIndex=QModelIndex());

    void save (void);
    void setSaved (bool isSaved);

protected:
    //
    // Overrides
    //
    virtual void closeEvent (QCloseEvent * event);
    virtual void contextMenuEvent (QContextMenuEvent * event);

private slots:
    //void on_pbOK_clicked();
    //void on_pbCancel_clicked();
    void addRubric (void);
    void editRubric (void);
    void delRubric (void);
    void copyFromRubric (void);

    void addRubricItem (void);
    void createRubricItem (void);
    void editRubricItem (void);
    void slotRubricItemDblClicked(const QModelIndex & index);
    void slotRubricItemEdit(const QModelIndex & index);
    void delRubricItem (void);

    void addSearchTemplateIntoRubric (void);
    void addCategoryIntoRubric (void);
    void rubricSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected);
    void turnRubricSplitter (void);
    void setRubricIcon (void);

public slots:
    void slotAddRubricItem(int idRubricItem, QString name);

private slots:
    void init();

signals:
    void rubricItemRequested();
    void rubricItemCreationRequested(const KKSRubric * r);
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
    QMenu * pMenu;

private:
    //
    // Functions
    //
    void initActions (void);
    const KKSRubric * currentRubric();
    const KKSRubric * getRubric(QModelIndex index);

    QModelIndex appendRubricRow(const KKSRubric * r, QModelIndex index);
    QModelIndex appendItemRow(const KKSRubricItem * item, QModelIndex index);
    void initTwIncludes();
    void parseRubric(KKSRubric * r, QModelIndex index);
    void parseItems(KKSRubric * r, QModelIndex index);
    void expandAllIndexes (const QModelIndex& parent) const;
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
