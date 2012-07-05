#ifndef _RubricForm_H
#define _RubricForm_H

#include <QDialog>
#include <kksgui_config.h>

namespace Ui
{
    class rubric_form;
};

class QAbstractItemModel;
class QModelIndex;

class KKSSearchTemplate;
class KKSCategory;
class KKSAccessEntity;

class KKSStuffForm;

class _GUI_EXPORT RubricForm : public QDialog
{
public:
    RubricForm (QString defaultRubricName=QString(), QString defaultRubricDesc=QString(), bool forRecords=false, QWidget* parent=0, Qt::WFlags f=0);
    ~RubricForm (void);

    QString getRubricName (void) const;
    void setRubricName (QString rName);

    KKSSearchTemplate* getSearchTemplate (void) const;
    void setSearchTemplate (KKSSearchTemplate * st);

    //QAbstractItemModel * getStuffModel (void) const;
    //void setStuffModel (QAbstractItemModel *stMod);
    KKSStuffForm * getStuffForm (void);
    void setStuffForm (KKSStuffForm * _sForm);

    KKSCategory * getCategory (void) const;
    void setCategory (KKSCategory * c);
    
    QString getRubricDesc (void) const;
    void setRubricDesc (QString rDesc);

    KKSAccessEntity * getAccessEntity (void) const;

public slots:
    void setAccessEntity (KKSAccessEntity * _acl);

private slots:
    void loadSearchTemplate (void);
    void loadCategory (void);
    //void currentPrivilegiesChanged (const QModelIndex& ind);

signals:
    void requestSearchTemplate (void);
    void requestCategory (void);

private:
    //
    // Functions
    //
    //void initPrivilegiesModel (void);

private:
    //
    // Variables
    //
    Ui::rubric_form * UI;
    KKSSearchTemplate * searchTemplate;
    KKSCategory * cat;
    KKSStuffForm * stuffForm;
    //KKSAccessEntity * acl;

private:
    Q_OBJECT
};

#endif
