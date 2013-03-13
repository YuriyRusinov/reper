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
		//Функция установки полного количества строк в виджете
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
		//Позиция курсора
        int pos_cursor;
		
        int pg_count;
		//Количество строк в курсоре
        int count_cursor;

        QScrollBar *scroll_view;                 //Линеечка:)
        SyncQueueViewerForm *syncQueueViewerForm;//Указатель на основную форму

		int totalRowCount;
};

#endif
