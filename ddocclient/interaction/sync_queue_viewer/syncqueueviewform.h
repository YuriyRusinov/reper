#ifndef _SYNCQUEUEVIEWFORM_H
#define _SYNCQUEUEVIEWFORM_H

#include <QDialog>

#include "syncqueueviewerform.h"

class SyncQueueViewerForm;
class QTreeWidget;

namespace Ui 
{
    class sync_queue_view_form;
}

class SyncQueueViewForm : public QDialog
{
    Q_OBJECT

    public:
        SyncQueueViewForm (QWidget * parent=NULL);
        ~SyncQueueViewForm (void);

        void InitView();
        void FillView();


    private slots:
        void on_pbUp_clicked();
        void on_pbDown_clicked();
        void on_pbCancel_clicked();
        void on_pbOk_clicked();

    private:
        //void ParserQString(QTreeWidget *treewidget, QString fullString);
        //void AddToTreeview(QTreeWidget *treewidget, QList< QPair<QString,QString> > list);

        Ui::sync_queue_view_form *ui;

        int cur_selection;
        int count_cursor;

        QString sqlCursor;
        SyncQueueViewerForm *syncQueueViewerForm;
};

#endif
