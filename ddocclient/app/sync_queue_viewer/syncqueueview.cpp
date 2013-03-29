#include "syncqueueview.h"

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

//*****�������� � ����������� ���������� ������*****
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
	if(i_value < 0 || i_value >= totalRowCount)
		return;

	if(i_value + viewRowCount -1 >= totalRowCount)
		i_value = totalRowCount - viewRowCount;

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
	{
		bottom = totalRowCount - 1;
		top = bottom - viewRowCount+1;
	}
	if(bottom < 0)
		bottom = -1;

	emit signal_viewRows(top,bottom);
}
//
//���� ��� ������� ������� �� ������ ���������
//
void SyncQueueView::slot_sliderPressed()
{
	if(!flag_scroll)
		flag_scroll = true;
}
//
//���� ����������� ������� �����
//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
//
void SyncQueueView::slot_viewRows()
{
	int top    = -1;
	int bottom = -1;

	if(model() && totalRowCount > 0)
	{
		scroll_view->setValue(0);

		//*****����������� �������� ������� �����*****
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
//���� ��������� ������� ������ ���������
//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
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