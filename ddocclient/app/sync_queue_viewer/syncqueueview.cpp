#include "syncqueueview.h"

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

//*****�������� � ����������� ���������� ������*****
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

//*****������� ��������� ������*****
//
//������� �������� �������� ����� ������� ������
//
bool SyncQueueView::getClearFlag() const
{
	return flag_clearAll;
}
//
//������� �������� ���������� ����� � �������
//
int SyncQueueView::getTotalRowCounts() const
{
	return totalRowCount;
}
//
//������� �������� ���������� ������� �����
//
int SyncQueueView::getViewRowCounts() const
{
	return viewRowCount;
}
//
//��������� ������� �������� value
//
int SyncQueueView::getOldValue() const
{
	return oldValue;
}
//**********

//*****������� ��������� ������*****
//
//������� ��������� ������� ���������� ����� � �������
//���� �������� �������, �� ������� false
//
bool SyncQueueView::setRowCount(int i_rowCount)
{
	//*****�������� �� ������������*****
	if(i_rowCount <= 0)
		return false;
	//**********

	totalRowCount = i_rowCount;

	return true;
}
//
//��������� ����� ������ ������� �������
//
void SyncQueueView::setClearFlag(bool i_flag)
{
	flag_clearAll = i_flag;
}
//
//��������� �������� ������ ���������
//
void SyncQueueView::setScrollValue(int i_value)
{
	if(i_value < totalRowCount)
		scroll_view->setSliderPosition(i_value);
}
//
//��������� ���������� ������� �����
//
void SyncQueueView::setViewRows(int i_value)
{
	viewRowCount = i_value;
}
//**********

//*****��������������� ������� *****
//
//������� ���������� ������ � ���������� �������
//
void SyncQueueView::updateData()
{
    setDirtyRegion(viewport()->rect());
}
//
//������� ������� �������
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

//*****�����*****
//
//���� ��� ������� ���������� ������ ���������
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
//���� ��� ������� ������� �� ������ ���������
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
//���� ����������� ������� �����
//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
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

		//*****����������� �������� ������� �����*****
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
//���� ��������� ������� ������ ���������
//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
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