#include "filtersform.h"

FiltersForm::FiltersForm(QWidget *parent)
	: QDialog(parent)
{
	setModal(true);

	setWindowTitle(tr("Data base view: filters"));

	//Создание интерфейса
	qgb_organizations = new QListView(this);//();
	qgb_objectType    = new QListView(this);//(tr("Object type"),this);
	qgb_result        = new QListView(this);//(tr("Result"),this);

	QStringList sl = QStringList();

	sl << tr("OD 1") << tr("OD 2");

	model_org = new QStringListModel(sl,this);
	qgb_organizations->setModel(model_org);
	qgb_organizations->setSelectionMode(QAbstractItemView::MultiSelection);
	select_org = qgb_organizations->selectionModel();

	sl.clear();
	sl << tr("Category (system)") << tr("IO (system)") << tr("EIO (directory record)")
	   << tr("EIO (record additional table directory)") << tr("Additional table directory")
	   << tr("Transfer request to the ping") << tr("Reserved")
	   << tr("Transfer complete directory of organizations to the new object")
	   << tr("Transferring directory entries for all organizations involved in the exchange of information objects") << tr("Public DL");

	model_obj = new QStringListModel(sl,this);
	qgb_objectType->setModel(model_obj);
	qgb_objectType->setSelectionMode(QAbstractItemView::MultiSelection);
	select_obj = qgb_objectType->selectionModel();

	sl.clear();
	sl << tr("Expected timing (standing in the transmission)")
	   << tr("Synchronized (exchange of information is not yet complete)")
	   << tr("Synchronization is successful")
	   << tr("An error has occurred");

	model_res = new QStringListModel(sl,this);
	qgb_result->setModel(model_res);
	qgb_result->setSelectionMode(QAbstractItemView::MultiSelection);
	select_res = qgb_result->selectionModel();

	qlb_organizations = new QLabel(tr("Organizations"),this);
	qlb_objectType = new QLabel(tr("Object type"),this);
	qlb_result = new QLabel(tr("Result"),this);

	qpb_ok     = new QPushButton(tr("Ok"),this);
	qpb_cancel = new QPushButton(tr("Cancel"),this);

	qcb_timeTo = new QCheckBox();
	qcb_timeFrom = new QCheckBox();

	qlb_from = new QLabel(tr("From:"),this);
	qlb_to   = new QLabel(tr("To:"),this);

	qdte_from = new QDateTimeEdit(this);
	qdte_to   = new QDateTimeEdit(this);

	qdte_from->setDisplayFormat("dd.MM.yyyy");
	qdte_to->setDisplayFormat("dd.MM.yyyy");

	qdte_to->setDate(QDate().currentDate());

	QGridLayout* qgl_date = new QGridLayout;
	qgl_date->addWidget(qcb_timeFrom,0,0,Qt::AlignRight);
	qgl_date->addWidget(qcb_timeTo,1,0,Qt::AlignRight);
	qgl_date->addWidget(qlb_from,0,1,Qt::AlignRight);
	qgl_date->addWidget(qlb_to,1,1,Qt::AlignRight);
	qgl_date->addWidget(qdte_from,0,2,Qt::AlignRight);
	qgl_date->addWidget(qdte_to,1,2,Qt::AlignRight);

	QGridLayout* qgl_groopbox  = new QGridLayout;
	QHBoxLayout* qhbl_buttons  = new QHBoxLayout;
	QVBoxLayout* qvbl_main     = new QVBoxLayout;

	qgl_groopbox->addWidget(qlb_organizations,0,0,Qt::AlignHCenter);
	qgl_groopbox->addWidget(qlb_objectType,0,1,Qt::AlignHCenter);
	qgl_groopbox->addWidget(qlb_result,0,2,Qt::AlignHCenter);
	qgl_groopbox->addWidget(qgb_organizations,1,0,Qt::AlignHCenter);
	qgl_groopbox->addWidget(qgb_objectType,1,1,Qt::AlignHCenter);
	qgl_groopbox->addWidget(qgb_result,1,2,Qt::AlignHCenter);

	QSplitter* qsh_datebutton = new QSplitter(Qt::Horizontal);

	qhbl_buttons->addLayout(qgl_date);
	qhbl_buttons->addWidget(qsh_datebutton);
	qhbl_buttons->addWidget(qpb_ok);
	qhbl_buttons->addWidget(qpb_cancel);

	qvbl_main->addLayout(qgl_groopbox);
	qvbl_main->addLayout(qhbl_buttons);
	setLayout(qvbl_main);

	//Подключение элементов интерфейса
	QObject::connect(qpb_ok,SIGNAL(clicked()),this,SLOT(accept()));
	QObject::connect(qpb_cancel,SIGNAL(clicked()),this,SLOT(reject()));
	QObject::connect(qcb_timeFrom,SIGNAL(clicked()),this,SLOT(slot_timeCheck()));
	QObject::connect(qcb_timeTo,SIGNAL(clicked()),this,SLOT(slot_timeCheck()));
}

FiltersForm::~FiltersForm()
{
	//delete select_org;
}

void FiltersForm::slot_timeCheck()
{
	if(!qcb_timeFrom->checkState())
	{
		qdte_from->setEnabled(true);
	}
	else
	{
		qdte_from->setEnabled(false);
	}

	if(!qcb_timeTo->checkState())
	{
		qdte_to->setEnabled(true);
	}
	else
	{
		qdte_to->setEnabled(false);
	}
}