#ifndef _SYNCQUEUEVIEW_H
#define _SYNCQUEUEVIEW_H

#include <QTreeView>
#include <QScrollBar>
#include <QHeaderView>
#include <QKeyEvent>

#include "sqv_delegate.h"
#include "dataelement.h"

class SyncQueueViewerForm;

class SyncQueueView : public QTreeView
{
	Q_OBJECT

    public:
        SyncQueueView(QWidget * parent);
        ~SyncQueueView();

		int getTotalRowCounts() const;
		int getViewRowCounts() const;

        bool setTotalRowCount(int i_rowCount);
        void setScrollValue(int i_value);

        void updateData();

    signals:
        void signal_visionScreen(sqv_data::window rhs);
        void signal_scrollScreen(sqv_data::scrollWindow rhs);

    public slots:
		void slot_sliderRealised();
        void slot_sliderPressed();
        void slot_sliderValueChanged(int value);

		void slot_viewRows();

	protected:
        virtual void keyPressEvent(QKeyEvent * i_event);
        virtual void resizeEvent(QResizeEvent * i_event);

    private slots:
        void slot_setNewScreen(sqv_data::window rhs);

    private:
        QScrollBar *scroll_view;
        bool flag_scrollPress;

        sqv_data::viewWindowOptions view;

        inline unsigned int visibleRowCount() const;
        inline unsigned int windowRowCount() const;

        inline int selectWindowChangeType(const sqv_data::window& rhs) const;
        inline bool detectNewScreen(const sqv_data::window& rhs) const;
        inline bool detectUpMovement(const sqv_data::window& rhs) const;
        inline bool detectDownMovement(const sqv_data::window& rhs) const;
};

#endif
