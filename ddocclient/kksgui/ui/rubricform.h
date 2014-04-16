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
class QPixmap;

class KKSSearchTemplate;
class KKSCategory;
class KKSObject;
class KKSAccessEntity;

class KKSStuffForm;

class _GUI_EXPORT RubricForm : public QDialog
{
public:
    RubricForm (QString defaultRubricName=QString(), 
                QString defaultRubricDesc=QString(), 
                bool forRecords=false, 
                QWidget* parent=0, 
                Qt::WFlags f=0);

    virtual ~RubricForm (void);

    bool forRecords() const {return m_forRecords;}

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
    
    KKSObject * getIO(void) const;
    void setIO (KKSObject * io);

    QString getRubricDesc (void) const;
    void setRubricDesc (QString rDesc);

    KKSAccessEntity * getAccessEntity (void) const;
    
    QString getIconAsString (void) const;
    void setIcon (const QPixmap& rubrIcon);
    
    void setCatEnabled (bool isEn);

public slots:
    void setAccessEntity (KKSAccessEntity * _acl);

private slots:
    void loadSearchTemplate (void);
    void loadCategory (void);
    void loadIO();
    void loadRubricImage (void);
    //void currentPrivilegiesChanged (const QModelIndex& ind);

signals:
    void requestSearchTemplate (void);
    void requestCategory (void);
    void requestIO();

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
    KKSObject * m_io;
    KKSStuffForm * stuffForm;
    bool m_forRecords;
    //KKSAccessEntity * acl;

private:
    Q_OBJECT
};

#endif
