/***********************************************************************
 * Module:  KKSObjEditorFactory.h
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 14:40:51
 * Purpose: Declaration of the class KKSObjEditorFactory
 * Comment: данный класс является фабрикой редакторов ИО
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSObjEditorFactory_h)
#define __KKSSITOOOM_KKSObjEditorFactory_h

//#include <KKSObjEditor.h>
//#include <KKSTemplate.h>
#include "kksfactory_config.h"

#include <QWidget>
#include <QItemSelection>

#include "KKSEntityFactory.h"
#include "KKSIndAttr.h"
#include <KKSList.h>
#include <KKSFilter.h>
#include <KKSMap.h>

class KKSObjEditor;
class KKSTemplate;
class KKSObject;
class KKSObjectExemplar;
class KKSLoader;
class KKSFileLoader;
class KKSFile;
class KKSEIOFactory;
class KKSPPFactory;
class KKSAttributesFactory;
class KKSCategory;
class KKSCategoryTemplateWidget;
class KKSCatEditor;
class KKSAttributesEditor;
class KKSCategoryAttr;
class KKSTemplateEditor;
class KKSXMLForm;
class KKSEIOData;
class KKSStuffFactory;
class PeriodicReportForm;
class KKSFiltersEditorForm;
class KKSRubricFactory;
class KKSSearchTemplate;
class KKSAttrGroup;
class KKSAttrValue;
class KKSIndFactory;
class KKSIndicator;
class KKSIndicatorValue;

class QScrollArea;
class QGridLayout;
class QModelIndex;
class QIODevice;
class QComboBox;
class QToolBar;
class QTabWidget;

class _F_EXPORT KKSObjEditorFactory : public KKSEntityFactory
{
public:
    KKSObjEditor* createObjEditor (int idObject,// идентификатор ИО, который будет содержать создаваемый (редактируемый) ЭИО (для ЭИО, которые являются ИО этот идентификатор должен быть равен IO_IO_ID)
                                   qint64 idObjE, // идентификатор создаваемого (редактируемого) ЭИО. Если ЭИО создается, должно быть равно -1
                                   const KKSList<const KKSFilterGroup *> & filters,// Применяется для ЭИО, которые являются контейнерными ИО. Содержит набор фильтров их таблицы
                                   const QString & extraTitle,
                                   const KKSCategory* wCat=0, // категория информационных объектов
                                   bool mode=true, // наличие кнопок OK, Cancel, Apply
                                   const QString& tableName = QString(), // название доп. таблицы, основная таблица соотвествует пустому значению
                                   bool bShowExecButton = false, //определяет наличие кнопки "Отправить адресату" при исполнении распоряжения. Во всех остальных случаях ее быть не должно
                                   Qt::WindowModality windowModality=Qt::NonModal, // модальность окна
                                   QWidget *parent=0,
                                   Qt::WindowFlags f=0);

    KKSObjEditor* createObjRecEditor (int idObject,// идентификатор ИО, который будет содержать создаваемый (редактируемый) ЭИО (для ЭИО, которые являются ИО этот идентификатор должен быть равен IO_IO_ID)
                                      qint64 idObjE, // идентификатор создаваемого (редактируемого) ЭИО. Если ЭИО создается, должно быть равно -1
                                      const KKSList<const KKSFilterGroup *> & filters,// Применяется для ЭИО, которые являются контейнерными ИО. Содержит набор фильтров их таблицы
                                      const QString & extraTitle,
                                      const KKSCategory* wCat=0, // категория информационных объектов
                                      bool mode=true, // наличие кнопок OK, Cancel, Apply
                                      bool toolB = false, // видимость тулбара
                                      Qt::WindowModality windowModality=Qt::NonModal, // модальность окна
                                      QWidget *parent=0,
                                      Qt::WindowFlags f=0);

    void insertReport (qint64 idObjE, QWidget *parent=0, Qt::WindowFlags f=0);

    void importCopies (KKSObject *io, // ИО, который будет содержать импортируемые ЭИО
                       const QStringList& attrCodeList,
                       const QList<QStringList>& oesList,
                       KKSObjEditor *oEditor,
                       const KKSCategory *cat=0,
                       const QString& tableName=QString());

    int exportHeader (QIODevice *xmlDev, // XML-файл, содержащий всю информацию
                      const KKSCategory *c, // категория, описывающая таблицу (io->category()->tableCategory())
                      QString codeName, // кодировка выходных данных
                      QString fDelim, // разделитель полей
                      QString tDelim, // разделитель текста
                      KKSObjEditor *oEditor // родительский редактор ИО и ЭИО
                      );

    int exportCopies (QIODevice *csvDev, // целевой CSV файл
                      const KKSCategory *c,
                      const KKSList<KKSObjectExemplar *>& oeData,
                      QString codeName, // кодировка выходных данных
                      QString fDelim, // разделитель полей
                      QString tDelim, // разделитель текста
                      KKSObjEditor *oEditor, // родительский редактор ИО и ЭИО
                      QString tableName // таблица экспорта
                      );

    int exportCopies (QIODevice *csvDev, // целевой CSV файл
                      const KKSCategory *c,
                      const KKSMap<qint64, KKSEIOData *>& oeData,
                      QString codeName, // кодировка выходных данных
                      QString fDelim, // разделитель полей
                      QString tDelim, // разделитель текста
                      KKSObjEditor *oEditor // родительский редактор ИО и ЭИО
                      );

    KKSSearchTemplate * loadSearchTemplate (void) const;
    KKSObjEditor* createObjEditorParam (int idObject,// идентификатор ИО, который будет содержать создаваемый (редактируемый) ЭИО (для ЭИО, которые являются ИО этот идентификатор должен быть равен IO_IO_ID)
                                        qint64 idObjE, // идентификатор создаваемого (редактируемого) ЭИО. Если ЭИО создается, должно быть равно -1
                                        const KKSList<const KKSFilterGroup *> & filters,// Применяется для ЭИО, которые являются контейнерными ИО. Содержит набор фильтров их таблицы
                                        const QString & extraTitle,
                                        const KKSCategory* wCat, // категория информационных объектов
                                        const KKSMap<int, KKSAttrValue *>& io_aVals, // список априорно заданных параметров атрибутов ИО
                                        const KKSMap<int, KKSAttrValue *>& aVals, // список априорно заданных параметров атрибутов ЭИО
                                        bool mode=true, // наличие кнопок OK, Cancel, Apply
                                        const QString& tableName = QString(), // название доп. таблицы, основная таблица соотвествует пустому значению
                                        bool bShowExecButton = false, //определяет наличие кнопки "Отправить адресату" при исполнении распоряжения. Во всех остальных случаях ее быть не должно
                                        Qt::WindowModality windowModality=Qt::NonModal, // модальность окна
                                        QWidget *parent=0,
                                        Qt::WindowFlags f=0
                                        );

    void createSearchTemplate();

public slots:
    void sendIO (KKSObject *wObj, KKSObjectExemplar *wObjE, KKSObjEditor *editor);
    void printReport (KKSObject* io);

private:
    //
    // Functions
    //
    friend class KKSSito;
    KKSObjEditorFactory (KKSPPFactory *_ppf=0,
                         KKSLoader *l=0,
                         KKSFileLoader * fl=0,
                         KKSAttributesFactory * awf=0,
                         KKSEIOFactory * _eiof=0);
    ~KKSObjEditorFactory();

    void setParams (KKSPPFactory *_ppf,
                    KKSLoader *l,
                    KKSFileLoader * fl,
                    KKSAttributesFactory * awf,
                    KKSEIOFactory * _eiof,
                    KKSStuffFactory * _sf);

    void setStuffFactory (KKSStuffFactory * _sf);

    void setRubrFactory (KKSRubricFactory * _rf);

    void setIndicesFactory (KKSIndFactory * _indf);

    int setAttributes (const KKSTemplate *t,
                             KKSObject *obj,
                             QWidget *attrWidget,
                             QGridLayout *gAttrLayout,
                             const KKSCategory *c,
                             KKSObjectExemplar *wObjE,
                             const QString& tableName,
                             KKSObjEditor *editor
                             );

    int setAttributes (const KKSTemplate *t,
                             KKSObject *obj,
                             QWidget *attrWidget,
                             QGridLayout *gAttrLayout,
                             const KKSCategory *c,
                             KKSObjEditor *editor,
                             bool updateView=false
                             );

/*    void setIndicators (KKSObject * obj,
                        QWidget * indWidget,
                        QGridLayout * gIndLay,
                        KKSObjEditor * editor,
                        bool updateView=false
                       );*/
    
    int setIndicators (const KKSTemplate *t,
                       KKSObject * obj,
                       QWidget * indWidget,
                       QGridLayout * gIndLay,
                       const KKSCategory *c,
                       KKSObjectExemplar *wObjE,
                       const QString& tableName,
                       KKSObjEditor * editor,
                       bool updateView=false
                      );

    void initActions (QToolBar *tBar, KKSObjEditor *editor);

    //данный метод вызывается, когда необходимо сделать запись в журнале контроля исполнения
    //т.е. сопоставить ИО и запись в ЖВР
    int insertInControlJournal(int idJournal, int idObject, bool draft = false);
    int updateInControlJournal(int idJournal, int idObject);

    //
    // данный метод проверяет является ли ИО экземпляр c idObjEx
    //
    bool isIO (int idObjEx) const;

    void setObjConnect (KKSObjEditor *editor);

signals:
    void editorCreated (KKSObjEditor * editor);
    void editorCreatedModal (KKSObjEditor * editor);
    void categoryEditorCreated (KKSCatEditor *cEditor);
    void categoryEditorCreatedModal (KKSCatEditor *cEditor);
    void attributesListLoaded (KKSAttributesEditor *aEditor);
    void categoryAdded (KKSCategory *cat);
    void templateEditorCreated (KKSTemplateEditor *tEditor);
    void templateEditorCreatedModal (KKSTemplateEditor *tEditor);
    void templateAdded (KKSTemplate *templ);
    void cioSaved (KKSObjectExemplar * cio);

private slots:
    void saveObject (KKSObjEditor * editor, KKSObject * wObj, KKSObjectExemplar * wObjEx, int num=1);
    void saveObjectAsCommandResult (KKSObjEditor* editor, KKSObject* wObj, KKSObjectExemplar* pObjectEx, int num=1);

    void saveObjectEx (KKSObjEditor * editor, KKSObjectExemplar *wObjE, const KKSCategory* cat, QString tableName, int num=1);
    void loadObject (KKSObjEditor * editor, int idObjectEx, int idObject);

public slots:
    void filterEIO (KKSObjEditor * editor, int idObject, const KKSCategory * cat=0, QString tableName=QString());
    void refreshEIO (KKSObjEditor * editor, int idObject, const KKSCategory * cat, QString tableName, QAbstractItemModel * sourceMod);
    void filterTemplateEIO (KKSObjEditor * editor, int idObject, const KKSCategory * cat=0, QString tableName=QString());
    void createNewEditor (QWidget * editor, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal);
    void createNewEditorParam (QWidget * editor, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal, const KKSMap<int, KKSAttrValue *>& ioAvals, const KKSMap<int, KKSAttrValue *>& aVals);
    
    void editExistOE (QWidget * editor, int idObject, qint64 idObjEx, const KKSCategory * c, QString tableName, int nTab, bool isModal);
    int deleteOE (QWidget * editor, int idObject, qint64 idObjEx, QString tableName, int drow);
    
    void addAttrSearchTemplate (void);
    void editAttrSearchTemplate (void);
    void delAttrSearchTemplate (void);

private slots:
    void setEIOTemplates (KKSObjEditor* editor, KKSObject* wObj);

    void importEIO (KKSObjEditor * editor, int idObject, const KKSCategory * c, QString tableName);
    void importCatAttrs (KKSCategory * c, const QStringList& catAttrs);
    void importCSV (QIODevice *csvDev, 
                    QString codeName, 
                    QString fDelim, 
                    QString tDelim, 
                    QAbstractItemModel *dataModel, 
                    KKSXMLForm *xmlForm);

    void exportEIO (KKSObjEditor * editor, int idObject, const KKSCategory * c, QString tableName);
    void updateEIOView (KKSObjEditor * editor, 
                        int idObject, 
                        const QList<qint64>& idObjEx, 
                        const QList<int>& row, 
                        const KKSCategory *c, 
                        const QString& tableName, 
                        int nTab);

    void regroupAttrs (QWidget *wIOAttr, 
                       QScrollArea *scIOattr, 
                       QWidget *ioAttrs, 
                       int idObj, 
                       const KKSCategory *c, 
                       KKSIndAttr::KKSIndAttrClass isSystem, 
                       KKSObjEditor *editor);

    void loadAttributeReference (QString tableName, QWidget *awAttr, int attrId);
    void loadExecReference (QAbstractItemModel *exModel);
    void addExecReference (QString tableName, QAbstractItemModel *exModel, int idAttr);

    KKSTemplate * getTemplate (const KKSCategory * c, bool withAllMandatories=false, QWidget * parent = 0);

    //
    // используется для загрузки файлов с сервера в локальную ФС
    //
    void slotDownloadFile (KKSFile *, const QWidget * parent);

    void slotIncludeRequested (KKSObjEditor * editor);//используется при редактировании вложений ИО. Вызывает в режиме диалога редактор справочника ИО
    void slotIncludeRecRequested (KKSObjEditor * editor);//используется при редактировании вложений ИО. Вызывает в режиме диалога редактор справочника ИО
    void slotOpenRubricItemRequested (int idObject, KKSObjEditor * editor);
    void slotOpenRubricItemRecRequested (int idObjectE, KKSObjEditor * editor);

    void addSendIO (void);

    void loadAttributeFilters (KKSAttribute * attr, QComboBox * cbList);
    void loadAttributeFilters (KKSAttribute * attr, QAbstractItemModel * mod);
    void saveSearchCriteria (KKSFilterGroup * group);

    void loadSearchCriteria (void);//QAbstractItemModel * mod);

    void addNewSearchTempl (QAbstractItemModel * searchMod);
    void addCopySearchTempl (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    void updateSearchTempl (const QModelIndex& wIndex, QAbstractItemModel * searchMod);
    void deleleSearchTempl (const QModelIndex& wIndex, QAbstractItemModel * searchMod);

    void addIOTable (KKSObject * wObj, KKSObjEditor * editor);

    void loadObjAttrRef (KKSObject * wObj, 
                         const KKSAttrValue* attr, 
                         KKSIndAttr::KKSIndAttrClass isSystem, 
                         QAbstractItemModel * sMod);
    void loadObjCAttrRef (KKSObjectExemplar * wObjE, 
                          const KKSAttrValue* attr, 
                          KKSIndAttr::KKSIndAttrClass isSystem, 
                          QAbstractItemModel * sMod);
    void loadObjDelAttrRef (KKSObject * wObj, 
                            const KKSAttrValue* attribute, 
                            KKSIndAttr::KKSIndAttrClass isSystem, 
                            QAbstractItemModel * sourceModel, 
                            const QModelIndex& wInd);
    void loadObjCDelAttrRef (KKSObjectExemplar * wObjE, 
                             const KKSAttrValue* attribute, 
                             KKSIndAttr::KKSIndAttrClass isSystem, 
                             QAbstractItemModel * sourceModel, 
                             const QModelIndex& wInd);
    void loadRefIO (QString tableName);

private:
    //
    // Functions
    //
    void initTemplateConnections (KKSTemplateEditor *tEditor);
    //KKSList<const KKSFilterGroup *> viewMainCategories (void);
    void updateAttrModel (const QModelIndex & wIndex, 
                          QAbstractItemModel * sMod, 
                          int id, 
                          QString tableName, 
                          KKSObject * refIO);
    int putAttrsGroupsOnWidget (KKSObject * obj, 
                                KKSObjectExemplar * wObjE, 
                                KKSObjEditor * editor, 
                                int& nc, 
                                const KKSCategory *c, 
                                QString tableName, 
                                KKSAttrGroup * aGroup, 
                                QGridLayout *gbLay, 
                                QGridLayout * gAttrLayout, 
                                bool isGrouped);
    int putRecAttrsGroupsOnWidget (KKSObject * obj, 
                                   KKSObjectExemplar * wObjE, 
                                   KKSObjEditor * editor, 
                                   int& nc, 
                                   const KKSCategory *c, 
                                   QString tableName, 
                                   KKSAttrGroup * aGroup, 
                                   QGridLayout *gbLay, 
                                   QGridLayout * gAttrLayout, 
                                   bool isGrouped);
    void putAttrsGroupsOnWidget (KKSObject * obj, 
                                 KKSObjEditor * editor, 
                                 int& nc, 
                                 const KKSCategory *c, 
                                 KKSAttrGroup * aGroup, 
                                 QGridLayout *gbLay, 
                                 QGridLayout * gAttrLayout, 
                                 bool isGrouped, 
                                 bool updateView);
    void putIndicatorsGroupsOnToWidget (KKSObject * obj, 
                                        KKSObjEditor * editor, 
                                        int& nc, 
                                        const KKSMap<int, KKSIndicatorValue*>& indMap, 
                                        QGridLayout *gbLay, 
                                        QGridLayout * gAttrLayout, 
                                        bool isGrouped, 
                                        bool updateView);
    void putRubricator (KKSObject * obj, 
                        KKSObjEditor * editor, 
                        QTabWidget * tabObj);
    void putRubricator (KKSObjectExemplar * eio, 
                        KKSObjEditor * editor, 
                        QTabWidget * tabObj);    
    void putSyncWidget (KKSObjEditor * editor, 
                        KKSObjectExemplar * wObjE, 
                        qint64 idObjE, 
                        QTabWidget * tabObj);
    void setIONameSecret (KKSObjEditor * editor, 
                          KKSObjectExemplar * wObjE, 
                          KKSObject * io, 
                          int& nWR, 
                          QGridLayout * mainLayout);
    
    void loadEntities (KKSObject *& obj, 
                       KKSObjectExemplar *& wObjE, 
                       const KKSCategory* wCat, 
                       int idObject, 
                       qint64 idObjE, 
                       const QString& tableName, 
                       const KKSTemplate *& tSystem, 
                       bool defTemplateOnly = false, 
                       QWidget * parent=0,
                       bool simplify = true);
    void loadRecEntities (KKSObject * obj, 
                          KKSObjectExemplar * wObjE, 
                          const KKSCategory* wCat, 
                          int idObject, 
                          qint64 idObjE, 
                          const QString& tableName, 
                          const KKSTemplate *& tRecAttr, 
                          bool defTemplateOnly = false, 
                          QWidget * parent=0);
    void loadEIOasIO (const KKSTemplate *& ioTemplate, 
                      KKSObject *& io, 
                      int idObject, 
                      qint64 idObjE, 
                      KKSObjectExemplar * wObjE, 
                      const KKSCategory* wCat);
    KKSTemplate * getRecordTemplate (KKSObject * io, 
                                     const KKSCategory* wCat);
    void initIOAttrs (KKSObject * io, 
                      KKSObjectExemplar * wObjE, 
                      const KKSCategory* wCat, 
                      KKSObjEditor * editor, 
                      QWidget * ioAttrWidget, 
                      QGridLayout * gIOLay);
    void setPreliminaryAttrs (KKSObject * io, 
                              const KKSMap<int, KKSAttrValue *>& aVals) const;
    void setPreliminaryAttrs (KKSObjectExemplar * cio, 
                              const KKSMap<int, KKSAttrValue *>& aVals) const;
    void getModelIds (QAbstractItemModel * mod, const QModelIndex& wIndex, QList<int>& ids) const;
    int searchParents (const KKSList<KKSObjectExemplar *>& oeList, 
                       const KKSCategoryAttr *cAttr, 
                       const KKSValue& refVal);

private:
    //
    // Variables
    //
    KKSPPFactory * ppf;
    KKSAttributesFactory * m_awf;
    KKSLoader * loader;
    KKSFileLoader * fileLoader;
    KKSEIOFactory * eiof;
    QItemSelection cSelection;
    KKSStuffFactory * m_sf;
    KKSRubricFactory * m_rf;
    KKSIndFactory * m_indf;

private:
    Q_OBJECT
};

#endif
