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

		//Установка базы данных
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
        KKSDatabase *db;		                 //База данных
        SyncQueueItemModel *model;               //Модель данных

		//Позиция курсора
        int pos_cursor;
		//
        int pg_count;
		//Количество строк в курсоре
        int count_cursor;

        QScrollBar *scroll_view;                 //Линеечка:)
        SyncQueueViewerForm *syncQueueViewerForm;//Указатель на основную форму

        int move_value;
        bool mousePress;
        QString sqlCursor;						 //Строка курсора

        QItemSelection itemSelection;
        SyncQueueItemModel *modelItem;           //Указатель на модель данных

	private slots:
		void slot_clicked(){};
		void slot_doublec_licked(){};
		void slot_expanded(){};
};

#endif
