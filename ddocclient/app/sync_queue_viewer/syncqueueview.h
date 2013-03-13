#ifndef _SYNCQUEUEVIEW_H
#define _SYNCQUEUEVIEW_H

#include <QTreeView>

class SyncQueueViewerForm;

class SyncQueueView : public QTreeView
{
	Q_OBJECT

    public:

        SyncQueueView(QWidget * parent);
        ~SyncQueueView();

		//
		//������� ��������� ������� ���������� ����� � �������
		//
		void setRowCount(int iRowCount){totalRowCount = iRowCount;};

        void UpdateData();

    signals:
		void signal_updateModelData(int output_topRow,int output_bottomRow);
		void signal_viewRows(int top,int bottom);

    public slots:
		void slot_sliderValueChanged(int value);

		void slot_resizeEvent();

    private:
		//������� �������
        int pos_cursor;
		
        int pg_count;
		//���������� ����� � �������
        int count_cursor;

        QScrollBar *scroll_view;                 //��������:)
        SyncQueueViewerForm *syncQueueViewerForm;//��������� �� �������� �����

		int totalRowCount;
};

#endif
