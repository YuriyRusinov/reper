#ifndef _SYNCQUEUEVIEW_H
#define _SYNCQUEUEVIEW_H

#include <QTreeView>

class SyncQueueViewerForm;
class SyncQueueItemModel;
class KKSDatabase;

class SyncQueueView : public QTreeView
{
	Q_OBJECT

    public:

        SyncQueueView(QWidget * parent);
        ~SyncQueueView();

        void SetDB(KKSDatabase *adb){db =adb;}
        void SetCountCursor(bool change);
        SyncQueueItemModel* GetModel(){return modelItem;}
        void SetCurrentPos(int apos_cursor){pos_cursor= apos_cursor;}
        void SetSQLCursor(QString asqlCursor){sqlCursor = asqlCursor;}
        void SetCountCursor(int acount){count_cursor = acount;}

        void UpdateData();

    public slots:

        void sliderRealised();
        void sliderPressed();

        void InitSyncQueueView();
    private:
        KKSDatabase *db;
        SyncQueueItemModel *model;

        int pos_cursor;
        int pg_count;
        int count_cursor;

        QScrollBar *scroll_view;
        SyncQueueViewerForm *syncQueueViewerForm;

        int move_value;
        bool mousePress;
        QString sqlCursor;

        QItemSelection itemSelection;
        SyncQueueItemModel *modelItem;
};

#endif
