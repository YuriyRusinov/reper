#ifndef _SYNCQUEUEVIEWRFORM_H
#define _SYNCQUEUEVIEWRFORM_H

#include <QMessageBox>
#include <QApplication>
#include <QDialog>

#include "kksdatabase.h"
#include "syncqueueitemmodel.h"
#include "syncqueueview.h"
#include "filtersform.h"
#include "dataelement.h"

class SyncQueueViewerForm : public QDialog
{
    Q_OBJECT

    public:
        SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent=NULL);
        ~SyncQueueViewerForm ();

	protected:
		virtual void resizeEvent(QResizeEvent * pe);


    private slots:
		void slot_filters_setup();

        void slot_viewClicked();

		void slot_accepted();
		void slot_rejected();

    signals:
        void signal_visionData();

	private:
		QPushButton* qpb_filters;
		QPushButton* qpb_delete;
		QPushButton* qpb_restart;
		QPushButton* qpb_cancel;
		QPushButton* qpb_save;
		QPushButton* qpb_exit;
		QPushButton* qpb_view;

        SQV_DataStream* dataPoint;

        SyncQueueView *syncQueueTreeWnd;

		SyncQueueItemModel * model;

        QString sqlCursorColumns;
		QString sqlCursorTF;
		QString sqlCursorFilters;  

        bool flag_clicked;

        int countColumn;  //Общее количество столбцов
        int countRow;	  //Общее количество строк

		int t_dataRow;
		int b_dataRow;

		FiltersForm* filtersDialog;

		QStringList org_list;
		QStringList obj_list;
		QStringList res_list;

		QString dateFrom;
		QString dateTo;

        inline void createInteface();
        inline void createConnections();

        inline void createFilters();
};

#endif
