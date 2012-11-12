#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <QtGui>
#include <QAbstractTableModel>
#include "ui_table_view.h"
#include "filter_dialog.h"
#include <kksdatabase.h>
#include <kkssito.h>

class QTreeView;
class KKSSito;

class table_view : public QWidget
{
	Q_OBJECT

public:
	table_view(QWidget *parent = 0, Qt::WFlags flags = 0);
	~table_view();

private:
	//Ui::table_viewClass ui;

	QPushButton *filter_button;
	QPushButton *delete_button;
	QPushButton *restart_button;
	QPushButton *cancel_button;
	QPushButton *file_l_button;
	QPushButton *exit_button;

	QTreeView *tree;
	QStandardItemModel *model;
	KKSDatabase* db;

private slots:
	void filter_dialog_exec();
};

#endif // TABLE_VIEW_H
