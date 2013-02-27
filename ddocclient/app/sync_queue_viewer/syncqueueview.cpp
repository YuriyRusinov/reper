#include "syncqueueview.h"

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

SyncQueueView::SyncQueueView(QWidget * parent):QTreeView(parent)
{
    scroll_view = verticalScrollBar();
    syncQueueViewerForm = (SyncQueueViewerForm *)(this->parent());

    connect((const QObject*)scroll_view, SIGNAL(sliderReleased()), this, SLOT(sliderRealised()));
    connect((const QObject*)scroll_view, SIGNAL(sliderPressed()),this, SLOT(sliderPressed()));

	//connect(this, SIGNAL(clicked()), this, SLOT(slot_clicked()));
    connect(this, SIGNAL(expanded()), this, SLOT(slot_expanded()));


	//Начальная позиция курсора
    pos_cursor = 0;
}

SyncQueueView::~SyncQueueView()
{
}

void SyncQueueView::UpdateData()
{
    modelItem->SetMove(false);
    setDirtyRegion(viewport()->rect());    
}

void SyncQueueView::sliderRealised()
{
    UpdateData();
}

void SyncQueueView::sliderPressed()
{
    modelItem->SetMove(true);
}

void SyncQueueView::InitSyncQueueView()
{   
    modelItem = new SyncQueueItemModel(count_cursor, _TABLE_COLUMN_COUNT_, db, sqlCursor);
    setModel(modelItem);

    for(int i=0; i < _TABLE_COLUMN_COUNT_; i++)
        resizeColumnToContents(i);
}
