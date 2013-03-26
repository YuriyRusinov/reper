#include "syncqueueview.h"

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

//*****Создание и уничтожение экземпляра класса*****
SyncQueueView::SyncQueueView(QWidget * parent):QTreeView(parent), totalRowCount(-1), viewRowCount(-1), flag_scroll(false), oldValue(-1), flag_clearAll(false)
{
	setModel(0);

	scroll_view = verticalScrollBar();

	connect((const QObject*)scroll_view, SIGNAL(sliderReleased()), this, SLOT(slot_sliderRealised()));
    connect((const QObject*)scroll_view, SIGNAL(sliderPressed()),this, SLOT(slot_sliderPressed()));

	connect((QObject*)scroll_view,SIGNAL(valueChanged(int)),this,SLOT(slot_sliderValueChanged(int)));
}

SyncQueueView::~SyncQueueView()
{
	setModel(0);
}
//**********

//*****Функции получения данных*****
//
//Функция возврата значения флага очистки данных
//
bool SyncQueueView::getClearFlag() const
{
	return flag_clearAll;
}
//
//Функция возврата количества строк в виджете
//
int SyncQueueView::getTotalRowCounts() const
{
	return totalRowCount;
}
//
//Функция возврата количества видимых строк
//
int SyncQueueView::getViewRowCounts() const
{
	return viewRowCount;
}
//
//Получение старого значения value
//
int SyncQueueView::getOldValue() const
{
	return oldValue;
}
//**********

//*****Функции установки данных*****
//
//Функция установки полного количества строк в виджете
//Если значение неверно, то вернуть false
//
bool SyncQueueView::setRowCount(int i_rowCount)
{
	//*****Проверка на соответствие*****
	if(i_rowCount <= 0)
		return false;
	//**********

	totalRowCount = i_rowCount;

	return true;
}
//
//Установка флага полной очистки виджета
//
void SyncQueueView::setClearFlag(bool i_flag)
{
	flag_clearAll = i_flag;
}
//
//Установка значения полосы прокрутки
//
void SyncQueueView::setScrollValue(int i_value)
{
	if(i_value < totalRowCount)
		scroll_view->setSliderPosition(i_value);
}
//
//Установка количества видимых строк
//
void SyncQueueView::setViewRows(int i_value)
{
	viewRowCount = i_value;
}
//**********

//*****Вспомогательные функции *****
//
//Функция обновления данных в выделенной области
//
void SyncQueueView::updateData()
{
    setDirtyRegion(viewport()->rect());
}
//
//Функция очистки виджета
//
void SyncQueueView::clear()
{
	totalRowCount = -1;
	viewRowCount  = -1;
	setModel(0);

	if(flag_clearAll)
	{
		oldValue = 0;
		flag_clearAll = false;
	}
}
//**********

//*****Слоты*****
//
//Слот для сигнала отпускания полосы прокрутки
//
void SyncQueueView::slot_sliderRealised()
{
	flag_scroll = false;

	int top    = -1;
	int bottom = -1;

	top = scroll_view->value();
	bottom = top + viewRowCount - 1;

	if(bottom >= totalRowCount)
		bottom = totalRowCount - 1;

	emit signal_viewRows(top,bottom);
}
//
//Слот для сигнала нажатия на полосу прокрутки
//
void SyncQueueView::slot_sliderPressed()
{
	if(!flag_scroll)
	{
		flag_scroll = true;

		oldValue = scroll_view->value();
	}
}
//
//Слот определения видимых строк
//В результате работы отправляется сигнал с номерами верхней и нижней строки
//
void SyncQueueView::slot_viewRows()
{
	int top    = -1;
	int bottom = -1;
	int flag_view = -1;

	if(model() && totalRowCount > 0)
	{
		oldValue = scroll_view->value();
		
		scroll_view->setValue(0);

		//*****Определение индексов видимых строк*****
		top = indexAt(QPoint(0,0)).row();

		QWidget * rect = viewport();

		bottom = indexAt(QPoint(0,rect->height())).row();
		//**********
		
		if(bottom < 0)
		{
			bottom = totalRowCount-1;
			flag_view = 0;
		}

		if(bottom >= top)
			viewRowCount = bottom - top + 1;
		else
			return;

		if(oldValue && flag_view)
		{
			if(oldValue+viewRowCount-1 < totalRowCount)
				flag_view = 1;
			else
				flag_view = 2;
		}
	}

	switch(flag_view)
	{
		case 1:
		{
			scroll_view->setValue(oldValue);
			top = oldValue;
			bottom = top + viewRowCount - 1; 
			break;
		}
		case 2:
		{
			top = oldValue + viewRowCount -1 - totalRowCount;
			top = oldValue - top;

			if(top < 0)
				top = 0;

			scroll_view->setValue(top);

			bottom = top + viewRowCount - 1;

			if(bottom >= totalRowCount)
				bottom = totalRowCount - 1;

			break;
		}
	}	

	emit signal_viewRows(top,bottom);
}
//
//Слот изменения позиции полосы прокрутки
//В результате работы отправляется сигнал с номерами верхней и нижней строки
//
void SyncQueueView::slot_sliderValueChanged(int value)
{
	if(flag_scroll)
		return;

	if(value - oldValue > 1)
		oldValue++;
	if(value - oldValue < -1)
		oldValue--;

	int top    = -1;
	int bottom = -1;

	top = value;
	bottom = top + viewRowCount - 1;

	if(bottom >= totalRowCount)
		bottom = totalRowCount - 1;

	emit signal_viewRows(top,bottom);

	updateData();
}
//**********