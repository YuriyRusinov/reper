#ifndef _SAVESET_FORM_H
#define _SAVESET_FORM_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QLineEdit>

#include "ui_save_settings_form.h"

class QAbstractItemModel;
class QAbstractItemDelegate;
class QSettings;

class SaveSettingsForm : public QDialog, private Ui::save_settings_form
{
    public:
        SaveSettingsForm (QSettings * set=NULL, QWidget * parent=NULL);
        virtual ~SaveSettingsForm ( void );

        QString curFile ( void ) const { return FileName; }
        void setFile ( const QString& name ) { FileName=name; }
//        void setModel (QAbstractItemModel * tModel) { this->treeView->setModel (tModel); }
//        QAbstractItemModel * getModel (void) { return this->treeView->model (); }

//        void setDelegate (QAbstractItemDelegate * tDeleg) { this->treeView->setItemDelegate (tDeleg); }
//        QAbstractItemDelegate * getDelegate (void) { return this->treeView->itemDelegate (); }

    public slots:
        void pbOkClicked ( void );

    private slots:
        void editSlot (const QModelIndex & index);

    private:
        void InitForm ( void );
//        void setModelData (QList<int>& ids, QList<QStringList>& dataval, int ntabs=0 );
        void setModelData (QAbstractItemModel *tModel, const QModelIndex& pIndex=QModelIndex());

    private:
        QSettings * settings;
        QString FileName;
        QModelIndexList changedIndexes;

    private:
        Q_OBJECT
};

#endif
