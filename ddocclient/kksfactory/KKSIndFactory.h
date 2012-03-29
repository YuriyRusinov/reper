#if !defined(__KKSSITOOOM_KKSIndFactory_h)
#define __KKSSITOOOM_KKSIndFactory_h

#include "kksfactory_config.h"

#include <QObject>
#include "KKSEntityFactory.h"

class QAbstractItemModel;
class QModelIndex;
class QGridLayout;
class QLabel;
class QWidget;
class QCheckBox;
class QToolButton;
class QTabWidget;
class QLineEdit;

class KKSLoader;
class KKSDatabase;
class KKSObjEditorFactory;
class KKSObject;
class KKSIndicatorValue;
class KKSIndicator;
class KKSAttrType;

class KKSObjEditor;
class KKSRecWidget;
class IndicatorForm;

class _F_EXPORT KKSIndFactory : public KKSEntityFactory
{
public:
    KKSRecWidget * getIOIndices (KKSObject * io, QWidget * parent=0, Qt::WindowFlags flags=0);
    void putIndWidget (KKSIndicatorValue *indicVal, KKSObjEditor *objEditor, QGridLayout *gLayout, int n_str, bool isSystem=false, QString tableName = QString(), int idCat=-1);
    void setValue (QWidget *aw, const KKSIndicator * pCategAttr, const KKSAttrType *pCatType, const QVariant& v, bool isObjExist, QString tableName, int idCat, QToolButton *tbRef, QCheckBox *ch, QWidget *wEditor, QLabel *lTitle, bool isRef=false);

private slots:
    void addIOIndex (KKSObject * io, const QModelIndex& pIndex, QAbstractItemModel * mod);
    void editIOIndex (KKSObject * io, const QModelIndex& wIndex, QAbstractItemModel * mod);
    void delIOIndex (KKSObject * io, const QModelIndex& wIndex, QAbstractItemModel * mod);
    void refreshIndices (KKSObject * io, QAbstractItemModel * mod);
    void viewIOIndicator (KKSObject * io, int idIndicator);
    void editIOIndicator (KKSObject * io, int idIndicator, QWidget * indW);
    void addNewIOIndicator (KKSObject * io, QWidget * indWidget);

    void addNewIndex (void);

    void loadIndType (KKSIndicatorValue * indVal, QLineEdit * lEIndType);
    void viewIndIOSRC (KKSIndicatorValue * indVal);
    void viewIndIOSRC1 (KKSIndicatorValue * indVal);
    void loadIndIOSRC (KKSIndicatorValue * indVal, QLineEdit * lEIndSrc);
    void loadIndIOSRC1 (KKSIndicatorValue * indVal, QLineEdit * lEIndSrc1);

signals:
    void viewIndicatorWidget (IndicatorForm * wIForm);

private:
    //
    // Functions
    //
    friend class KKSSito;
    KKSIndFactory (KKSDatabase * _db, KKSLoader *l, QObject *parent=0);
    ~KKSIndFactory (void);

    void setOEF (KKSObjEditorFactory * _oef);

    QAbstractItemModel * getIndicesModel (KKSObject * io);
    QLabel * createAttrTitle (const QString& title);
    QCheckBox * createChDateTime (QGridLayout *gLayout, QLabel *lTitle, int n_str);
    QWidget * createIndWidget (const KKSIndicator * pCategAttr, const KKSAttrType *pCatType,  bool isSystem, QGridLayout *gLayout, int n_str, const QVariant& V, QLabel *lTitle, QToolButton *&tbRef, QCheckBox *&ch, KKSObjEditor * objEditor, bool isRef=false);

    QWidget * createAttrCheckWidget (const KKSIndicator * pCategAttr, const KKSAttrType *pCatType, QTabWidget * tabW);

    void connectToSlots (QObject *aw, QWidget* wEditor);

private:
    //
    // Variables
    //
    KKSDatabase * db;
    KKSLoader * loader;
    KKSObjEditorFactory * oef;
private:
    Q_OBJECT
};

#endif
