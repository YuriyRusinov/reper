#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H

#include <QMessageBox>
#include <QApplication>
#include <QDialog>

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueview.h"
#include "filtersform.h"

namespace TableView
{
	const int TABLE_COLUMN_COUNT      = 17; //Количество столбцов в таблице находящейся в базе данных
	const int TABLE_COLUMN_COUNT_VIEW = 8;  //Количество столбцов в отображаемой таблице
}

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
		//*****Создание и уничтожение экземпляра класса*****
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();
		//**********

	protected:
		//
		//Обработчик события изменения размера
		//
		virtual void resizeEvent(QResizeEvent * pe);
		//virtual void mousePressEvent ( QMouseEvent * i_event );
		//virtual void mouseReleaseEvent ( QMouseEvent * i_event );
		//virtual void leaveEvent(QEvent * i_event);
		//virtual void enterEvent (QEvent * i_event);

    private slots:
		//
		//Слот вызова диалога для установки фильтров
		//
		void slot_filters_setup();
		//
		//Слот обращения к базе данных
        //
		void slot_viewClicked();
		//
		//Слот обновления данных модели
		//
		void slot_updateModelData(int i_topRow,int i_bottomRow);

		void slot_accepted();
		void slot_rejected();

	private:
		//
		//Возвращение числа колонок в курсоре
        //
		int countInCursor();
		//
		//Функция открытия курсора
		//
		bool openCursor();
		//
		//Функция получения данных из базы данных
		//
		int DBdata(int i_topRow,int i_bottomRow);
		//
		//Установка значения полосы прокрутки
		//
		void setScrollValue(int i_value);
		//
		//Функция закрытия курсора
		//
		void closeCursor();
		//
		//Функция инициализации виджета
		//
		void initWidget();
		//
		//Функция обновления данных в виджете
		//
		void refreshData();

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
		//
		//Переменная для хранения запроса
		//
        QString sqlCursorColumns;
		QString sqlCursorTF;
		QString sqlCursorFilters;
		//
		//Указатель на базу данных
        //
		KKSDatabase * db;	  

		//*****Флаги*****
        bool cursor_open; //Флаг открытия курсора

		bool flag_clicked;//Флаг нажатой кнопки
		bool flag_error;  //Флаг ошибки
		//**********

        int countColumn;  //Общее количество столбцов
        int countRow;	  //Общее количество строк

		int t_dataRow;
		int b_dataRow;

		FiltersForm* filtersDialog;

		QStringList org_list;
		QStringList obj_list;
		QStringList res_list;

		QString dateFrom;
		QString dateTo;
};

#endif
