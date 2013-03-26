#ifndef _SYNCQUEUEVIEW_H
#define _SYNCQUEUEVIEW_H

#include <QTreeView>
#include <QScrollBar>

class SyncQueueViewerForm;

class SyncQueueView : public QTreeView
{
	Q_OBJECT

    public:
		//*****�������� � ����������� ���������� ������*****
        SyncQueueView(QWidget * parent);
        ~SyncQueueView();
		//**********

		//*****������� ��������� ������*****
		//
		//������� �������� �������� ����� ������� ������
		//
		bool getClearFlag() const;
		//
		//������� �������� ���������� ����� � �������
		//
		int getTotalRowCounts() const;
		//
		//������� �������� ���������� ������� �����
		//
		int getViewRowCounts() const;
		//
		//��������� ������� �������� value
		//
		int getOldValue() const;
		//**********

		//*****������� ��������� ������*****
		//
		//������� ��������� ������� ���������� ����� � �������
		//���� �������� �������, �� ������� false
		//
		bool setRowCount(int i_rowCount);
		//
		//��������� ����� ������ ������� �������
		//
		void setClearFlag(bool i_flag);
		//
		//��������� �������� ������ ���������
		//
		void setScrollValue(int i_value);
		//
		//��������� ���������� ������� �����
		//
		void setViewRows(int i_value);
		//**********

		//*****��������������� ������� *****
		//
		//������� ���������� ������ � ���������� �������
		//
        void updateData();
		//
		//������� ������� �������
		//
		void clear();
		//**********

    signals:
		void signal_viewRows(int top,int bottom);

    public slots:
		//
		//���� ��� ������� ���������� ������ ���������
		//
		void slot_sliderRealised();
		//
		//���� ��� ������� ������� �� ������ ���������
		//
        void slot_sliderPressed();
		//
		//���� ����������� ������� �����
		//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
		//
		void slot_viewRows();
		//
		//���� ��������� ������� ������ ���������
		//� ���������� ������ ������������ ������ � �������� ������� � ������ ������
		//
		void slot_sliderValueChanged(int value);

    private:
		QScrollBar *scroll_view;//��������:)
		bool flag_scroll;       //���� ������� ������ ���������

		int totalRowCount;      //���������� ����� � �������
		int viewRowCount;       //���������� ������������ �����

		int oldValue;           //������ �������� value
		bool flag_clearAll;     //���� ������ �������
};

#endif
