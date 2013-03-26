#ifndef _SYNCQUEUEVIEW_H
#define _SYNCQUEUEVIEW_H

#include <QTreeView>
#include <QScrollBar>

class SyncQueueViewerForm;

class SyncQueueView : public QTreeView
{
	Q_OBJECT

    public:
		//*****Создание и уничтожение экземпляра класса*****
        SyncQueueView(QWidget * parent);
        ~SyncQueueView();
		//**********

		//*****Функции получения данных*****
		//
		//Функция возврата значения флага очистки данных
		//
		bool getClearFlag() const;
		//
		//Функция возврата количества строк в виджете
		//
		int getTotalRowCounts() const;
		//
		//Функция возврата количества видимых строк
		//
		int getViewRowCounts() const;
		//
		//Получение старого значения value
		//
		int getOldValue() const;
		//**********

		//*****Функции установки данных*****
		//
		//Функция установки полного количества строк в виджете
		//Если значение неверно, то вернуть false
		//
		bool setRowCount(int i_rowCount);
		//
		//Установка флага полной очистки виджета
		//
		void setClearFlag(bool i_flag);
		//
		//Установка значения полосы прокрутки
		//
		void setScrollValue(int i_value);
		//
		//Установка количества видимых строк
		//
		void setViewRows(int i_value);
		//**********

		//*****Вспомогательные функции *****
		//
		//Функция обновления данных в выделенной области
		//
        void updateData();
		//
		//Функция очистки виджета
		//
		void clear();
		//**********

    signals:
		void signal_viewRows(int top,int bottom);

    public slots:
		//
		//Слот для сигнала отпускания полосы прокрутки
		//
		void slot_sliderRealised();
		//
		//Слот для сигнала нажатия на полосу прокрутки
		//
        void slot_sliderPressed();
		//
		//Слот определения видимых строк
		//В результате работы отправляется сигнал с номерами верхней и нижней строки
		//
		void slot_viewRows();
		//
		//Слот изменения позиции полосы прокрутки
		//В результате работы отправляется сигнал с номерами верхней и нижней строки
		//
		void slot_sliderValueChanged(int value);

    private:
		QScrollBar *scroll_view;//Линеечка:)
		bool flag_scroll;       //Флаг нажатой полосы прокрутки

		int totalRowCount;      //Количество строк в виджете
		int viewRowCount;       //Количество отображаемых строк

		int oldValue;           //Старое значение value
		bool flag_clearAll;     //Флаг полной очистки
};

#endif
