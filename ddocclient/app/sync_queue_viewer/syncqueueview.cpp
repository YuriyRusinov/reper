#include "syncqueueview.h"

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

SyncQueueView::SyncQueueView(QWidget * parent):QTreeView(parent)
{
    scroll_view = verticalScrollBar();
    syncQueueViewerForm = (SyncQueueViewerForm *)(this->parent());

	connect((const QObject*)scroll_view, SIGNAL(valueChanged(int)), this, SLOT(slot_sliderValueChanged(int)));

	//Начальная позиция курсора
    pos_cursor = 0;
}

SyncQueueView::~SyncQueueView()
{
}

void SyncQueueView::UpdateData()
{
    setDirtyRegion(viewport()->rect());    
}

void SyncQueueView::slot_resizeEvent()
{
	int top = 0;
	int bottom = 0;

	top = indexAt(QPoint(0,0)).row();

	QWidget * rect = viewport();

	bottom = indexAt(QPoint(0,rect->height())).row();

	if(bottom == -1)
		bottom = totalRowCount;

	emit signal_viewRows(top,bottom);
}

void SyncQueueView::slot_sliderValueChanged(int value)
{
	slot_resizeEvent();
	UpdateData();
}