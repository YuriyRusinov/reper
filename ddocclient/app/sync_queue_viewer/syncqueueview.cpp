#include "syncqueueview.h"

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

//*****Создание и уничтожение экземпляра класса*****
SyncQueueView::SyncQueueView(QWidget * parent):QTreeView(parent), totalRowCount(-1), viewRowCount(-1), flag_scroll(false), flag_clearAll(false)
{
	setModel(0);

	scroll_view = verticalScrollBar();

	scroll_view->setSingleStep(1);
	scroll_view->setPageStep(3);

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
	if(i_value < 0 || i_value >= totalRowCount)
		return;

	if(i_value + viewRowCount -1 >= totalRowCount)
		i_value = totalRowCount - viewRowCount;

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
	{
		bottom = totalRowCount - 1;
		top = bottom - viewRowCount+1;
	}
	if(bottom < 0)
		bottom = -1;

	emit signal_viewRows(top,bottom);
}
//
//Слот для сигнала нажатия на полосу прокрутки
//
void SyncQueueView::slot_sliderPressed()
{
	if(!flag_scroll)
		flag_scroll = true;
}
//
//Слот определения видимых строк
//В результате работы отправляется сигнал с номерами верхней и нижней строки
//
void SyncQueueView::slot_viewRows()
{
	int top    = -1;
	int bottom = -1;

	if(model() && totalRowCount > 0)
	{
		scroll_view->setValue(0);

		//*****Определение индексов видимых строк*****
		top = indexAt(QPoint(0,0)).row();

		QWidget * rect = viewport();

		bottom = indexAt(QPoint(0,rect->height())).row();
		//**********
		
		if(bottom < 0)
		{
			bottom = totalRowCount-1;
		}

		if(bottom >= top)
			viewRowCount = bottom - top + 1;
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

	int top    = -1;
	int bottom = -1;

	top = value;
	bottom = top + viewRowCount - 1;

	if(bottom >= totalRowCount)
	{
		bottom = totalRowCount - 1;
		top = bottom - viewRowCount+1;
	}
	emit signal_viewRows(top,bottom);

	updateData();
}
//**********

void SyncQueueView::keyPressEvent(QKeyEvent * i_event)
{
	if(i_event->key() == Qt::Key_PageDown || i_event->key() == Qt::Key_PageUp)
		return;

	QTreeView::keyPressEvent(i_event);
}