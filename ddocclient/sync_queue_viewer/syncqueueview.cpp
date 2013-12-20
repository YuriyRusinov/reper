#include "syncqueueview.h"

SyncQueueView::SyncQueueView(QWidget * parent)
    :QTreeView(parent)
{
    view.totalRowCount = -1;
    view.visibleWindow.topRowIndex = -1;
    view.visibleWindow.bottomRowIndex = -1;

    flag_scrollPress = false;

    setModel(0);

	scroll_view = verticalScrollBar();
	scroll_view->setSingleStep(1);
	scroll_view->setPageStep(3);

	sqv_delegate* qid_height = new sqv_delegate(this); 
	setItemDelegate(qid_height);

	connect((const QObject*)scroll_view, SIGNAL(sliderReleased()), this, SLOT(slot_sliderRealised()));
    connect((const QObject*)scroll_view, SIGNAL(sliderPressed()),this, SLOT(slot_sliderPressed()));

    connect(this,SIGNAL(signal_visionScreen(sqv_data::window)),this,SLOT(slot_setNewScreen(sqv_data::window)));

	connect((QObject*)scroll_view,SIGNAL(valueChanged(int)),this,SLOT(slot_sliderValueChanged(int)));
}

SyncQueueView::~SyncQueueView()
{
	setModel(0);
}

int SyncQueueView::getTotalRowCounts() const
{
    return view.totalRowCount;
}

int SyncQueueView::getViewRowCounts() const
{
    return visibleRowCount();
}

bool SyncQueueView::setTotalRowCount(int i_rowCount)
{
	if(i_rowCount <= 0)
		return false;

    view.totalRowCount = i_rowCount;
    //scroll_view->setMaximum(view.totalRowCount);

	return true;
}

void SyncQueueView::setScrollValue(int i_value)
{
    if(i_value < 0 || i_value >= view.totalRowCount)
        return;

    if(i_value + windowRowCount() >= view.totalRowCount)
        i_value = view.totalRowCount - windowRowCount();

    scroll_view->setSliderPosition(i_value);
}

void SyncQueueView::updateData()
{
    setDirtyRegion(viewport()->rect());
}

void SyncQueueView::slot_sliderRealised()
{
    flag_scrollPress = false;

    sqv_data::window visible;
    visible.topRowIndex = -1;
    visible.bottomRowIndex = -1;

    visible.topRowIndex = scroll_view->value();
    visible.bottomRowIndex = visible.topRowIndex + windowRowCount() - 1;

    emit signal_visionScreen(visible);
}

void SyncQueueView::slot_sliderPressed()
{
    if(!flag_scrollPress)
        flag_scrollPress = true;
}

void SyncQueueView::slot_viewRows()
{
    sqv_data::window visible;
    visible.topRowIndex = -1;
    visible.bottomRowIndex = -1;

    visible.topRowIndex = scroll_view->value();
    visible.bottomRowIndex = visible.topRowIndex + windowRowCount() - 1;

    emit signal_visionScreen(visible);
}

void SyncQueueView::slot_sliderValueChanged(int value)
{
    if(flag_scrollPress)
		return;

    sqv_data::window visible;
    visible.topRowIndex = -1;
    visible.bottomRowIndex = -1;

    visible.topRowIndex = value;
    visible.bottomRowIndex = visible.topRowIndex + windowRowCount() - 1;

    emit signal_visionScreen(visible);
}

void SyncQueueView::keyPressEvent(QKeyEvent * i_event)
{
	if(i_event->key() == Qt::Key_PageDown || i_event->key() == Qt::Key_PageUp)
		return;

	QTreeView::keyPressEvent(i_event);
}

void SyncQueueView::resizeEvent(QResizeEvent * i_event)
{
	QTreeView::resizeEvent(i_event);

	setColumnWidth(0,150);
	setColumnWidth(1,115);
	setColumnWidth(2,70);
	setColumnWidth(3,60);
	setColumnWidth(4,150);
	setColumnWidth(5,150);
	setColumnWidth(6,150);
	setColumnWidth(7,150);
}

void SyncQueueView::slot_setNewScreen(sqv_data::window rhs)
{
    sqv_data::scrollWindow window;

    switch(selectWindowChangeType(rhs))
    {
        case sqv_data::newWindow:
        {
            window.visibleWindow = rhs;
            window.flag_offsetPosition = sqv_data::newWindow;
            window.offset = 0;

            view.visibleWindow = window.visibleWindow;

            break;
        }
        case sqv_data::newScreen:
        {
            window.visibleWindow = rhs;
            window.flag_offsetPosition = sqv_data::newScreen;
            window.offset = 0;

            view.visibleWindow = window.visibleWindow;

            break;
        }
        case sqv_data::goUp:
        {
            window.visibleWindow = rhs;
            window.flag_offsetPosition = sqv_data::goUp;
            window.offset = abs(rhs.topRowIndex - view.visibleWindow.topRowIndex);

            break;
        }
        case sqv_data::goDown:
        {
            window.visibleWindow = rhs;
            window.flag_offsetPosition = sqv_data::goDown;
            window.offset = abs(rhs.topRowIndex - view.visibleWindow.topRowIndex);

            break;
        }
        default:
        {
            return;
        }
    }

    emit signal_scrollScreen(window);
}

inline unsigned int SyncQueueView::visibleRowCount() const
{
    return static_cast<unsigned int>(view.visibleWindow.topRowIndex - view.visibleWindow.bottomRowIndex + 1);
}

inline unsigned int SyncQueueView::windowRowCount() const
{
    unsigned int height = this->rect().height();
    unsigned int countRow = height / sqv_data::ROW_HEIGHT;

    if((height % sqv_data::ROW_HEIGHT))
        countRow++;

    return countRow;
}

inline int SyncQueueView::selectWindowChangeType(const sqv_data::window& rhs) const
{
    int type = sqv_data::noMovement;

    if(view.visibleWindow.topRowIndex < 0 && view.visibleWindow.bottomRowIndex < 0)
    {
        type = sqv_data::newWindow;
        return type;
    }
    if(detectNewScreen(rhs))
    {
        type = sqv_data::newScreen;
        return type;
    }
    if(detectUpMovement(rhs))
    {
        type = sqv_data::goUp;
        return type;
    }
    if(detectDownMovement(rhs))
    {
        type = sqv_data::goDown;
        return type;
    }

    return type;
}

inline bool SyncQueueView::detectNewScreen(const sqv_data::window& rhs) const
{
    if(windowRowCount() != visibleRowCount())
        return true;
    if(abs(rhs.topRowIndex - view.visibleWindow.topRowIndex) >= windowRowCount())
        return true;

    return false;
}

inline bool SyncQueueView::detectUpMovement(const sqv_data::window& rhs) const
{
    if(rhs.topRowIndex - view.visibleWindow.topRowIndex < 0)
        return true;

    return false;
}

inline bool SyncQueueView::detectDownMovement(const sqv_data::window& rhs) const
{
    if(rhs.topRowIndex - view.visibleWindow.topRowIndex > 0)
        return true;

    return false;
}
