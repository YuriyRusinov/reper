/***********************************************************************
 * Module:  KKSObjEditor.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:05
 * Purpose: Declaration of the class KKSObjEditor
 * Comment: Данный класс унаследован от QWidget и предназначен для визуализации и редактирования ИО или ЭИО
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSObjEditor_h)
#define __KKSSITOOOM_KKSObjEditor_h

#include "kksgui_config.h"

#include <QWidget>
#include <QMap>
#include <QMultiMap>
#include <QModelIndex>
//#include "KKSWidget.h"

#include "KKSAttribute.h"
#include "KKSIndAttr.h"
#include "KKSMap.h"
#include "KKSList.h"
#include "KKSFilter.h"
#include "KKSDialog.h"
#include "KKSRecDialog.h"

class QPushButton;
class QGridLayout;
class QTreeView;
class QAbstractItemModel;
class QBoxLayout;
class QComboBox;
class QCheckBox;
class QToolButton;
class QScrollArea;
class QItemSelection;
class QToolBar;
class QTabWidget;
class QLineEdit;
class QLabel;

class KKSTemplate;
class KKSView;
class KKSObjectExemplar;
class KKSAttrType;
class KKSValue;
class KKSCategory;
class KKSWidget;
class KKSObject;
class KKSCategoryAttr;
class KKSRecWidget;
class KKSFileWidget;
class KKSIncludesWidget;
class KKSPrivilege;
class KKSAddTable;
class RubricForm;
class KKSAccessEntity;
class KKSStuffForm;
class KKSIndicator;
class KKSAttrValue;

class _GUI_EXPORT KKSObjEditor : public KKSRecDialog
{
   public:
        KKSObjEditor (const KKSTemplate *t, //шаблон для системных параметров (т.е. как для ЭИО)
                      const KKSTemplate *_ioTemplate, //шаблон для пользовательских атрибутов (т.е. как для ИО)
                      const KKSTemplate * rTemplate, // шаблон для записей справочника
                      const KKSCategory * ioCat, // категория вновь создаваемого ИО (т.е. именно ИО)
                      const KKSList<const KKSFilterGroup*> & filters, //фильтры, применяемые к KKSView
                      const QString & extraTitle, //если к KKSView применены фильтры, то данный параметр может содержать уточнение об этом
                      bool mode=true, //наличие кнопок ОК и т.д.
                      KKSObjectExemplar *objEx=0, //создаваемый или редактируемы ЭИО
                      KKSObject * obj=0, //если редактируемый ЭИО является ИО, то это он
                      bool isIO = false,//определяет, что редактируется/создается именно ИО
                      QToolBar * tBar=0,
                      QWidget *parent=0, 
                      Qt::WindowFlags f=0);
        virtual ~KKSObjEditor (void);

        const KKSTemplate* sysTemplate (void) const;
        void setSysTemplate (const KKSTemplate *_sysT);
        const KKSTemplate* ioTemplate (void) const;
        void setIoTemplate (const KKSTemplate *_ioT);
        const KKSTemplate* recTemplate (void) const;
        void setRecTemplate (const KKSTemplate *_rT);

        const KKSList<const KKSFilterGroup*> & filters() const;
        void setFilters (const KKSList<const KKSFilterGroup *>& filters);
        void clearFilters (void);

        void set_object_ex_data (KKSObjectExemplar* ioObjectE);
        KKSObjectExemplar* getObjectEx (void);
        void setObj(KKSObject* ioObj);
        KKSObject* getObj(void);
        const KKSCategory * getCategory (void) const;

        //
        // если ИО создается как результат выполнения записи (команды) в ЖВР,
        // то в журнале контроля распоряжений необходимо сделать соответствующую запись
        // при этом надо связать создаваемый ИО и заданную запись журнала.
        // Кроме того, надо учесть, что запись может быть сохранена как черновик. 
        // В этом случае отправки адресату (id_jr_state = 4) быть не должно
        //
        void setIdJournal(int idJournal);
        int idJournal() const;
        bool asDraft() const;
        void setAsDraft(bool draft = true);

        bool showExecButton() const;
        void setShowExecButton(bool yes = true);

        //int getID (void) const;

        void save(int num=1);

        void print();

        bool isObjChanged (void) const;
        void setObjChanged (bool val);

        QTabWidget * getRecTab (void) const;
        void setRecTab (QTabWidget * tabEnc);
        
    public slots:
        void setValue (qint64 id, KKSIndAttrClass sys, QVariant val);
        //void setIndValue (int id, bool sys, QVariant val);

        void loadAttrHistory (const KKSAttrValue * av, bool forRecs);
    private slots:

        void accept (void);
        
        int apply (int num=1);

        void addObjectE (void);
        void editObjectE (void);
        void delObjectE (void);
        void filterObjectE(void);
        void filterObjectT (void);
        void importObjectE (void);
        void exportObjectE (void);
        void setView (void);
        void refreshRecModel (QAbstractItemModel * sourceMod);

        void saveChildObjE (KKSObjectExemplar *childObjE);
        void updateEIOEx (KKSObject * refObj, const KKSMap<qint64, KKSObjectExemplar *>& recs, QAbstractItemModel * recModel, const QModelIndex& recIndex);

        void setEnableO (int state);

        void setList (void);
        void setStateList (void);
        void childWidget (KKSObjEditor *editor);
        void setAttrView (void);

        void currentRecordChanged (const QModelIndex& current, const QModelIndex& previous);
        void currentRecSelChanged (const QItemSelection& selected, const QItemSelection& deselected);

        void rubricsChanged (void);
        void attrValueChanged (void);
        void filesChanged();

        void sendIO (void);
        void saveAsCommandResult (void);

        void editCurrentRec (const QModelIndex& index);
        void privChanged (KKSPrivilege *p, KKSObject * obj);
        void addAnotherTable (void);

        void addAttributeCheckReference (const KKSAttrValue* av, KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel);
        void delAttributeCheckReference (const KKSAttrValue* av, KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd);
        
        void openReferenceIO (QString tableName);
        void openReferenceRec (QString tableName, qint64 id );

        void setIOGlobal (bool isGlobal);
        void setSyncType (QLineEdit * leType);

        void addSyncOrg (KKSAttrValue * av, QAbstractItemModel * sMod);
        void delSyncOrg (KKSAttrValue * av, const QModelIndex& oInd, QAbstractItemModel * sMod);

        void setAccessIO (KKSAccessEntity * acl);

        void addIndex (QAbstractItemModel * sourceMod, const QModelIndex& pIndex);
        void editIndex (QAbstractItemModel * sourceMod, const QModelIndex& wIndex);
        void delIndex (QAbstractItemModel * sourceMod, const QModelIndex& wIndex);
        void refreshIndices (QAbstractItemModel * sourceMod);

        void viewIndicator (void);
        void generateIOUUID (qint64 idAttrVal);
        void setIOUUID (QString uuid);
        
        void recSaved (KKSObjectExemplar * rec);
        void viewAHist (const KKSAttrValue *, const KKSList<KKSAttrValue *>& histList);
//        void editIndicator (void);
//        void delIndicator (void);
//        void addIndicator (void);

    signals:
        void filterObjectEx(KKSObjEditor*, int idObject, const KKSCategory * c, QString tableName);
        void refreshObjectEx(KKSObjEditor*, int idObject, const KKSCategory * c, QString tableName, QAbstractItemModel * sourceMod);
        void filterObjectTemplateEx(KKSObjEditor*, int idObject, const KKSCategory * c, QString tableName);
        void newObjectEx (QWidget*, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * sRecMod);
        void editObjectEx (QWidget*, int idObject, qint64 idObjE, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * sRecMod, const QModelIndex& recIndex);
        void delObjectEx (QWidget*, int idObject, qint64 idObjE, QString tableName, QAbstractItemModel * recModel, const QModelIndex& recIndex);
        void saveObjE (KKSObjEditor*, KKSObjectExemplar * wObjE, const KKSCategory * cat, QString tableName, int num, QAbstractItemModel * recModel);
        void loadObjE (int id, QString tableName);
        void eioChanged (KKSObject * refObj, const KKSMap<qint64, KKSObjectExemplar *>& recs, QAbstractItemModel * recMod, const QModelIndex&);
        void updateEIO(KKSObject * refObj, const KKSMap<qint64, KKSObjectExemplar *>& objRecs, QAbstractItemModel * recModel, const QItemSelection& sel, const QModelIndex& recInd);
        
        void saveObj (KKSObjEditor*, KKSObject* wObj, KKSObjectExemplar * wObjEx, int num, QAbstractItemModel * recModel);
        void saveObjAsCommandResult(KKSObjEditor*, KKSObject* wObj, KKSObjectExemplar * wObjEx, int num, QAbstractItemModel * recModel);
        
        void loadAttrRef (QString tableName, QWidget * awAttr, int attrId);//посылаем сигнал всегда с ИД атрибута. Поскольку используется в соответствующем слоте именно он
        void loadStateRef (KKSObjEditor* editor, QWidget * stateW, KKSObjectExemplar * wObjEx);
        void setTemplate (KKSObjEditor* editor, KKSObject* wObj);
        void importObjectEx (KKSObjEditor * editor, int idObject, const KKSCategory * c, QString tableName, QAbstractItemModel * recModel);
        void exportObjectEx (KKSObjEditor * editor, int idObject, const KKSCategory * c, QString tableName, QAbstractItemModel * recModel);

        void rubricItemSelected(int idObject, QString name);
        void rubricItemRecSelected(int idObjectRec, QString name);
        void includeRequested(KKSObjEditor * editor);
        void includeRecRequested(KKSObjEditor * editor);
        void openRubricItemRequested(int idObject, KKSObjEditor * editor);
        void openRubricItemRecRequested(int idObjectE, KKSObjEditor * editor);

        void updateAttributes (QWidget *ioAttrWidget, QScrollArea *sc, QWidget *ioAttrsW, /*KKSObject * wObj*/ int idObj, const KKSCategory * wCat, KKSIndAttrClass isSystem, KKSObjEditor* editor);
        void closeEditor (void);
        void prepareIO (KKSObject* wObj, KKSObjectExemplar * wObjEx, KKSObjEditor* editor);
        void addAnotherTable (KKSObject * wObj, KKSObjEditor* editor);
        
        void editObjAttrRef (KKSObject * wObj, const KKSAttrValue* av, KKSIndAttrClass isSystem, QAbstractItemModel * sMod);
        void editObjCAttrRef (KKSObjectExemplar * wObjE, const KKSAttrValue* av, KKSIndAttrClass isSystem, QAbstractItemModel * sMod);
        void delObjAttrRef (KKSObject * wObj, const KKSAttrValue* av, KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd);
        void delObjCAttrRef (KKSObjectExemplar * wObjE, const KKSAttrValue* av, KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd);
        
        void openRefIO (QString tableName);
        void openRefRec (QString tableName, qint64);

        void printReport(KKSObject * pObj); //сигнал ловится в классе KKSObjectFactory. Выводит на печать шаблон при помощи встроенного генератора шаблонов

        void addIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel*);
        void editIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel*);
        void delIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel*);
        void refreshIOIndex (KKSObject *, QAbstractItemModel*);
        void viewIOIndicator (KKSObject *, int);
        void editIOIndicator (KKSObject *, int, QWidget *);
        void addIOIndicator (KKSObject *, QWidget *);
        
        void generateUUID (int idAttr, const KKSAttrValue * av);
        void setUUID (QString uuid);
        
        void loadHistory (const KKSAttrValue *, qint64, qint64, bool);
        
        void viewHist (const KKSList<KKSAttrValue *>&);

    public slots:
        //void slotRubricItemRequested();
        void slotIncludeSelected(int idObject, QString name);
        void slotIncludeRequested();
        void slotOpenRubricItemRequested(int idObject);
        void slotIncludeRecSelected(int idObjectE, int idObject, QString name);
        void slotIncludeRecRequested();
        void slotOpenRubricItemRecRequested(int idObjectE);
        void setNumCopies (int n);

   protected:
        //
        // Overrides
        //
        virtual void paintEvent (QPaintEvent * event);
        virtual void closeEvent (QCloseEvent * event);

    private:
        //
        // Functions
        //
        void setObjectEx (KKSObjectExemplar *newObjEx);

        QGridLayout *getLayout (void) { return mainLayout; }
    public:
        //
        // Functions
        //
        void clearAttributes (void);
        void setSysAttrValue( KKSAttrValue* av);
        void setIOAttrValue ( KKSAttrValue* av);
        void setIndValue (KKSAttrValue *av);
        KKSMap<qint64, KKSAttrValue*> & getSysAttrValues();
        KKSMap<qint64, KKSAttrValue*> & getIOAttrValues();
        KKSMap<qint64, KKSAttrValue*> & getRecAttrValues();


        void clearOptionals (void);
        void clearSysOpts (void);
        void clearW (void);
        void clearIndicators (void);
        void addWidth (int w);

        void clearCbList (void);

        //void setRecordsWidget (KKSRecWidget *rw);
        //KKSRecWidget* getRecordsWidget (void) const;
        void addRecordsWidget (KKSRecWidget *rw);
        void addAdditionalCategory (const KKSCategory * wCat);
        void addAdditionalTable (KKSAddTable * wTable);
        void setAdditionalTables (const KKSMap<int, KKSAddTable*>& wTables);
        void setCurrentTable (const QString& tName);
        void setParentTab (int nt);
        int addCatsCount (void) const;
        void addListAttrWidget (QToolButton *tb, QWidget *aw, const KKSAttrValue* av);
        void setOpt (int id, KKSIndAttrClass isSystem, QCheckBox* ch);
        void addOptWidget (int id, KKSIndAttrClass isSystem, QWidget *w);
        
        void setRecordsModel (QAbstractItemModel * recMod);
        void setRecordsIndex (const QModelIndex& recInd);
/*
        void addTbQList (QToolButton * tbView, int idIndicator);
        void addTbEditList (QToolButton * tbView, int idIndicator);
        void addTbDelList (QToolButton * tbDel, int idIndicator);
        
        void addIndRow (int idIndicator, QWidget *editor, QLabel * lab);
*/
    private:
        //
        // Functions
        //
        void addFileWidget (KKSFileWidget *fw);
        void addIncludesWidget (KKSIncludesWidget *iw);
        void addIncludesRecWidget (KKSIncludesWidget *iw);
        void setSysAttrWidgets (QWidget *wSysAttr, QScrollArea *scSysAttr, QWidget *sysAttrs);
        void setIOAttrWidgets (QWidget *wIOAttr, QScrollArea *scIOattr, QWidget *ioAttrs);
        void setRecIndWidgets (QWidget *wIOAttr, QScrollArea *scIOattr, QWidget *ioAttrs);
        void setTabWidget (QTabWidget *tObj);

        void clearAdditionalCopies (void);
        void addAdditionalCopy (KKSObjectExemplar * rec);
        int constructObject (void);
        void saveToDb (int num=1);

        void setIndicatorNumber (int num);
        
        void setStateWidget (QWidget * w);

    private:
        friend class KKSObjEditorFactory;
        friend class KKSViewFactory;
        //
        // Variables
        //
        const KKSTemplate* m_sysTemplate;
        const KKSTemplate* m_ioTemplate;
        const KKSTemplate* m_recTemplate;
        KKSObjectExemplar* pObjectEx;
        KKSObject * pObj;
        const KKSCategory* pCat;
        KKSList <const KKSCategory*> addCats;
        KKSMap<int, KKSAddTable*> addTables;
        QString currTablename;
        int nTable;
        bool m_isIO;
        //если ИО создается как результат выполнения команды ЖВР, 
        //то данное поле хранит идентификатор записи журнала
        int m_idJournal;
        bool m_draft;
        bool m_showExecButton;

        KKSList<const KKSFilterGroup*> m_filters;

        //KKSMap<int, const KKSCategoryAttr*> sysAttributes;
        KKSMap<qint64, KKSAttrValue *> sysAttrValues;
        //QMap<int, QVariant> sysAttributesValues;
        QMap<QCheckBox*, int> chSysOpt;
        QMultiMap<int, QWidget*> chSysOptWidgets;

        //KKSMap<int, const KKSCategoryAttr*> ioAttributes;
        KKSMap<qint64, KKSAttrValue *> ioAttrValues;
        //QMap<int, QVariant> ioAttributesValues;
        QMap<QCheckBox*, int> chIOOpt;
        QMultiMap<int, QWidget*> chIOOptWidgets;

        QMap<QToolButton*, int> tbMaps;
        QMap<int, QWidget*> awAttrs;
        KKSMap<int, const KKSAttrValue*> listAttrValues;

        int indNumW;
        KKSMap<qint64, KKSAttrValue *> ioIndicatorValues;
        QMap<QCheckBox*, int> chIndOpt;
        QMultiMap<int, QWidget*> chIndOptWidgets;
        QMap<QToolButton *, int> tbViews;
        int numCopies;

        QList<int> hAttrWidths;

        //
        // Qt visual components
        //
        QPushButton *pbOk;
        QPushButton *pbCancel;
        QPushButton *pbApply;
        QToolBar *tbActions;

        QGridLayout *mainLayout;

        QTabWidget * tabObj;
        QTabWidget * tabEnclosures;

        QWidget *sysAttrWidget;
        QScrollArea *scSysAttrs;
        QWidget *sysAttrsW;

        QWidget *ioAttrWidget;
        QScrollArea *scIOAttrs;
        QWidget *ioAttrsW;

        QWidget *recIndWidget;
        QScrollArea *scRecInd;
        QWidget *recIndW;

        QWidget * stateWidget;

        //KKSRecWidget * recWidget;
        QList <KKSRecWidget *> addRecWidgets;
        KKSFileWidget * fileWidget;
        KKSIncludesWidget * includesWidget;
        KKSIncludesWidget * includesRecWidget;

        bool isChanged;
        QMap<int, int> editRows;
        KKSMap<qint64, KKSObjectExemplar *> additionalCopies;
        
        QAbstractItemModel * pRecModel;
        QModelIndex pRecIndex;

        QLabel * activeLabel;
    private:
        Q_OBJECT

};

#endif
