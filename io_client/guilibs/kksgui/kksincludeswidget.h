#ifndef KKSINCLUDESWIDGET_H
#define KKSINCLUDESWIDGET_H

#include <KKSDialog.h>
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

class _GUI_EXPORT KKSIncludesWidget: public KKSDialog
{
    Q_OBJECT

public:

    //владелец рубрикатора
    //рубрикатор может быть сам по себе (весь общесистемный рубрикатор, мои документы)
    //рубрикатор может содержаться в категории
    //в информационном объекте
    //в записи справочника
    enum RubricatorSource{
        rsRubricator = 0,
        rsMyDocs = 1,
        rsCategory = 2,
        rsIO = 3,
        rsRecord = 4
    };


    KKSIncludesWidget (KKSRubric * rootRubric,
                       RubricatorSource rSource = rsRubricator,
                       //bool isDocs=false,
                       //bool forCategory=false,
                       //bool forRecord=true,
                       QWidget *parent = 0,
                       Qt::WindowFlags flags = 0);
    
    ~KKSIncludesWidget();

    KKSRubric * rootRubric() const;

    void hideIncludes (void);
    void showIncludes (void);

    QTreeView *tvRubr (void);
    
    QAbstractItemModel * rubrModel () const;
    void setRubrModel (QAbstractItemModel * rModel);
    
    QAbstractItemModel * rubrItemsModel (void) const;
    
    void addRubricIntoModel (KKSRubric * cRubr, const QModelIndex& pIndex=QModelIndex());

    void save (void);
    void setSaved (bool isSaved);

    const KKSRubric * getSelectedRubric (void);
    
    bool isRubrics (void) const; //виджет создается только для просмотра рубрик без вложений (виджет с вложениями становится невидимым)
    void setForRubrics (bool isr);

protected:
    virtual void closeEvent (QCloseEvent * event);
    virtual void contextMenuEvent (QContextMenuEvent * event);

private slots:
    void addRubric (void);
    void editRubric (void);
    void delRubric (void);
    void copyFromRubric (void);

    void addRubricItem (void);
    void createRubricItem (QAbstractItemModel * itemModel, const QModelIndex& parent);
    void editRubricItem ();
    void editRubricDoc (QAbstractItemModel * itemModel, const QModelIndex& index);
    void editSelectedDocs (QAbstractItemModel * itemModel, const QItemSelection& selDocs);
    void delSelectedDocs (QAbstractItemModel * itemModel, const QItemSelection& selDocs);
    void slotRubricItemDblClicked(const QModelIndex & index);
    void slotRubricItemEdit(const QModelIndex & index);
    void delRubricItem (void);

    void addSearchTemplateIntoRubric (void);
    void addCategoryIntoRubric (void);
    void addIOIntoRubric (void);

    void rubricSelectionChanged (const QItemSelection& selected, const QItemSelection& deselected);
    void turnRubricSplitter (void);
    void setRubricIcon (void);
    
    void setSyncSettings (void);
    void putIntoAnotherRubric (void);
    void sendIOS (void);
    void setAccessRules (void);
    
    void refreshRubricItems (QAbstractItemModel * sourceMod);

public slots:
    void slotAddRubricItem(int idRubricItem, QString name);
    void slotInitAttachmentsModel (QAbstractItemModel * attachModel, KKSIncludesWidget * iW);

private slots:
    void init();
    void showReportEditor(qint64 idReport);
    void showReportViewer(qint64 idReport);

signals:

    void signalShowReportViewer(qint64 idReport);
    void signalShowReportEditor(qint64 idReport);

    void rubricRequested(KKSRubric *r, int idRubr, QAbstractItemModel * rubrModel, const QModelIndex& rubrIndex);
    void rubricCategoryRequested (KKSRubric *r);
    void rubricItemRequested(const KKSRubric * r, bool isRec, QAbstractItemModel * itemModel);
    void rubricItemCreationRequested(const KKSRubric * r, QAbstractItemModel * itemModel, const QModelIndex& parent);
    
    void openRubricItemRequested(int idRubricItem);
    void openRubricItemRequested(int idObject, int idRecord);
    
    void saveRubric (KKSRubric * r, bool isMyDocs);
    void rubricsChanged (void);
    void copyFromRubr (KKSRubric * rDest, QAbstractItemModel * attachModel, const QModelIndex& index);

    void loadStuffModel (RubricForm * rForm);
    void loadSearchtemplate (RubricForm * rForm);
    void loadCategory (RubricForm * rForm);
    void loadIO(RubricForm * rForm);
    void rubricAttachmentsView (QAbstractItemModel * attachModel, const KKSRubric * r);
    
    void initAttachmentsModel (const KKSRubric * r, bool forRecs);
    void appendRubricItemIntoModel (QAbstractItemModel * attachModel, const KKSRubricItem * rItem);
    
    void setSyncIO (const QList<int>& ioIDList);
    void putIOSIntoRubr (const QList<int>& ioIDList, const KKSRubric* cRubric);
    void sendIOS (const QList<int>& ioIDList);
    void setAccessIOS (const QList<int>& ioIDList);

private:
    //
    // Variables
    //
    KKSRubric * m_rootRubric;
    RubricatorSource m_rSource; //владелец рубрикатора (см. выше описание перечисления)
    //bool isMyDoc;
    //bool isRec;
    bool isChanged;
    
    bool rubricsOnly;

    QToolBar * tBRubrActions;
    QSplitter * spRubrics;
    QTreeView * twIncludes;
    KKSRecWidget * recWItems;
    QTreeView * tvItems;
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
    
    QList<int> getSelectedIOS (void) const;
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
