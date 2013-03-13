#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H

#include<QDialog>

#include "syncqueueitemmodel.h"

class KKSDatabase;
class SyncQueueView;

namespace TableView
{
	const int TABLE_COLUMN_COUNT = 16; //Количество столбцов в таблице
}

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();

		//
		//Возвращение строки запроса курсора
        //
		QString GetCursorString(){return sqlCursor;}
		//
		//Возвращение числа колонок в курсоре
        //
		int countInCursor();

        int GetColumnCount(){return countColumn;}
        int GetRowCount(){return countRow;}

        SyncQueueView* GetTreeView(){ return syncQueueTreeWnd;}
    private:
		//
		//Кнопки управления
		//
		QPushButton* qpb_filters;
		QPushButton* qpb_delete;
		QPushButton* qpb_restart;
		QPushButton* qpb_cancel;
		QPushButton* qpb_save;
		QPushButton* qpb_exit;
		QPushButton* qpb_view;

		//
		//Виджет отображения данных
		//
        SyncQueueView *syncQueueTreeWnd;
		//
		//Модель отображения данных
		//
		SyncQueueItemModel * model;

        QString sqlCursor;	  //Переменная для хранения запроса

        KKSDatabase * db;	  //Указатель на базу данных

        bool cursor_open_file;//Флаг открытия файла курсора
        bool cursor_open;     //Флаг открытия курсора

        int countColumn;	  //Количество столбцов
        int countRow;		  //Количество строк
        int count_colomn_logfile;

    private slots:
		//
		//Слот вызова диалога для установки фильтров
		//
		void slot_filters_setup();
		//
		//Слот обращения к базе данных
        //
		void on_pbView_clicked();
		//
		//Слот обновления данных модели
		//
		void slot_updateModelData(int input_topRow,int input_bottomRow);

	private:
		//
		//Функция получения данных из базы данных
		//
		void DBdata(int input_topRow,int input_bottomRow);
};

#endif
