#include "table_view.h"
#include <kkssito.h>

table_view::table_view(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	//ui.setupUi(this);
	filter_button = new QPushButton;
	delete_button = new QPushButton;
	restart_button = new QPushButton;
	cancel_button = new QPushButton;
	file_l_button = new QPushButton;
	exit_button = new QPushButton;

	filter_button->setText("Filter");
	delete_button->setText("Delete");
	restart_button->setText("Restart");
	cancel_button->setText("Cancel");
	file_l_button->setText("Save as...");
	exit_button->setText("Exit");

	tree = new QTreeView;

	QVBoxLayout* pvbxLayoutAddr = new QVBoxLayout;
	QHBoxLayout* phbxLayoutAddr = new QHBoxLayout;

	phbxLayoutAddr->addWidget(filter_button);
	phbxLayoutAddr->addWidget(delete_button);
	phbxLayoutAddr->addWidget(restart_button);
	phbxLayoutAddr->addWidget(cancel_button);
	phbxLayoutAddr->addWidget(file_l_button);
	phbxLayoutAddr->addWidget(exit_button);
	pvbxLayoutAddr->addLayout(phbxLayoutAddr);
	pvbxLayoutAddr->addWidget(tree);

	setLayout(pvbxLayoutAddr);

	connect(exit_button,SIGNAL(clicked()),this,SLOT(close()));
	connect(filter_button,SIGNAL(clicked()),this,SLOT(filter_dialog_exec()));

	model = new QStandardItemModel;
	model->setColumnCount(9);

    tree->setModel(model);

	int ok = kksSito->GUIConnect();
	if( ok != 1)
		QMessageBox::information(0,"Information","kksSito,ok_code!");

	db = kksSito->db();
}

table_view::~table_view()
{
	//delete model;
}

void table_view::filter_dialog_exec()
{
	Filter_dialog *filterDialog = new Filter_dialog;

	if(filterDialog->exec() == QDialog::Accepted)
	{
		KKSResult * res = db->execute("select * from out_sync_queue");

		int row = res->getRowCount();
		int column = res->getColumnCount();

		for (int row_index = 0; row_index < row; ++row_index)
		{
			for (int column_index = 0; column_index < column; ++column_index) 
			{
				QStandardItem *item = new QStandardItem(res->getCellAsString(row_index,column_index));
				model->setItem(row_index, column_index, item);
			}
		}
	}

	delete filterDialog;
}