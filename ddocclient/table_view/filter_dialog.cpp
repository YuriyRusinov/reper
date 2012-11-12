#include "filter_dialog.h"

Filter_dialog::Filter_dialog(QWidget *parent)
	: QDialog(parent)
{
	QPushButton *ok_button = new QPushButton("Ok");
	QPushButton *cancel_button = new QPushButton("Cancel");

	connect(ok_button,SIGNAL(clicked()),this,SLOT(accept()));
	connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));

	QLabel* lblDate_main  = new QLabel("Date");
	QLabel* lblDate_start = new QLabel("from:");
	QLabel* lblDate_end   = new QLabel("to:");

	QHBoxLayout* phbxLayoutButton = new QHBoxLayout;
	QVBoxLayout* pvbxLayoutDate1  = new QVBoxLayout;
	QVBoxLayout* pvbxLayoutDate2  = new QVBoxLayout;
	QHBoxLayout* phbxLayoutDate   = new QHBoxLayout;
	QVBoxLayout* pvbxGlobalLayout = new QVBoxLayout;

	pvbxLayoutDate1->addWidget(lblDate_main);
	pvbxLayoutDate1->addStretch();
	pvbxLayoutDate2->addWidget(lblDate_start);
	pvbxLayoutDate2->addWidget(lblDate_end);
	phbxLayoutDate->addLayout(pvbxLayoutDate1);
	phbxLayoutDate->addLayout(pvbxLayoutDate2);

	phbxLayoutButton->addWidget(ok_button);
	phbxLayoutButton->addWidget(cancel_button);

	QTabBar* ptbEdit = new QTabBar;

	ptbEdit->addTab("Organisation");
	ptbEdit->addTab("Entity");
	ptbEdit->addTab("Result");

	pvbxGlobalLayout->addWidget(ptbEdit);

	pvbxGlobalLayout->addLayout(phbxLayoutDate);
	pvbxGlobalLayout->addLayout(phbxLayoutButton);

	setLayout(pvbxGlobalLayout);
}

Filter_dialog::~Filter_dialog()
{

}
