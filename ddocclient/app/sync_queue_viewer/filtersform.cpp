#include "filtersform.h"

FiltersForm::FiltersForm(QWidget *parent)
	: QDialog(parent)
{
	setModal(true);

	//Создание интерфейса
	qgb_organizations = new QGroupBox(tr("Organization"),this);
	qgb_objectType    = new QGroupBox(tr("Object type"),this);
	qgb_result        = new QGroupBox(tr("Result"),this);

	qcb_org1 = new QCheckBox(tr("OD 1"));
	qcb_org2 = new QCheckBox(tr("OD 2"));

	QVBoxLayout* qvbl_organizations = new QVBoxLayout;
	qvbl_organizations->addWidget(qcb_org1);
	qvbl_organizations->addWidget(qcb_org2);
	qgb_organizations->setLayout(qvbl_organizations);

	qcb_type1  = new QCheckBox(tr("Category (system)"));
	qcb_type2  = new QCheckBox(tr("IO (system)"));
	qcb_type3  = new QCheckBox(tr("EIO (directory record)"));
	qcb_type4  = new QCheckBox(tr("EIO (record additional table directory)"));
	qcb_type5  = new QCheckBox(tr("Additional table directory"));
	qcb_type6  = new QCheckBox(tr("Transfer request to the ping"));
	qcb_type7  = new QCheckBox(tr("Reserved"));
	qcb_type8  = new QCheckBox(tr("Transfer complete directory of organizations to the new object"));
	qcb_type9  = new QCheckBox(tr("Transferring directory entries for all organizations involved in the exchange of information objects"));
	qcb_type10 = new QCheckBox(tr("Public DL"));

	QVBoxLayout* qvbl_objectType = new QVBoxLayout;
	qvbl_objectType->addWidget(qcb_type1);
	qvbl_objectType->addWidget(qcb_type2);
	qvbl_objectType->addWidget(qcb_type3);
	qvbl_objectType->addWidget(qcb_type4);
	qvbl_objectType->addWidget(qcb_type5);
	qvbl_objectType->addWidget(qcb_type6);
	qvbl_objectType->addWidget(qcb_type7);
	qvbl_objectType->addWidget(qcb_type8);
	qvbl_objectType->addWidget(qcb_type9);
	qvbl_objectType->addWidget(qcb_type10);
	qgb_objectType->setLayout(qvbl_objectType);

	qcb_resultType1 = new QCheckBox(tr("Expected timing (standing in the transmission)"));
	qcb_resultType2 = new QCheckBox(tr("Synchronized (exchange of information is not yet complete)"));
	qcb_resultType3 = new QCheckBox(tr("Synchronization is successful"));
	qcb_resultType4 = new QCheckBox(tr("An error has occurred"));

	QVBoxLayout* qvbl_result = new QVBoxLayout;
	qvbl_result->addWidget(qcb_resultType1);
	qvbl_result->addWidget(qcb_resultType2);
	qvbl_result->addWidget(qcb_resultType3);
	qvbl_result->addWidget(qcb_resultType4);
	qgb_result->setLayout(qvbl_result);

	qpb_ok     = new QPushButton(tr("Ok"),this);
	qpb_cancel = new QPushButton(tr("Cancel"),this);

	qlb_from = new QLabel(tr("From:"));
	qlb_to   = new QLabel(tr("To:"));

	qdte_from = new QDateTimeEdit;
	qdte_to   = new QDateTimeEdit;

	QGridLayout* qgl_date = new QGridLayout;
	qgl_date->addWidget(qlb_from,0,0,Qt::AlignRight);
	qgl_date->addWidget(qlb_to,1,0,Qt::AlignRight);
	qgl_date->addWidget(qdte_from,0,1,Qt::AlignRight);
	qgl_date->addWidget(qdte_to,1,1,Qt::AlignRight);

	QHBoxLayout* qhbl_groopbox = new QHBoxLayout;
	QHBoxLayout* qhbl_buttons  = new QHBoxLayout;
	QVBoxLayout* qvbl_main     = new QVBoxLayout;

	qhbl_groopbox->addWidget(qgb_organizations);
	qhbl_groopbox->addWidget(qgb_objectType);
	qhbl_groopbox->addWidget(qgb_result);

	QSplitter* qsh_datebutton = new QSplitter(Qt::Horizontal);

	qhbl_buttons->addLayout(qgl_date);
	qhbl_buttons->addWidget(qsh_datebutton);
	qhbl_buttons->addWidget(qpb_ok);
	qhbl_buttons->addWidget(qpb_cancel);

	qvbl_main->addLayout(qhbl_groopbox);
	qvbl_main->addLayout(qhbl_buttons);
	setLayout(qvbl_main);

	//Подключение элементов интерфейса
	QObject::connect(qpb_ok,SIGNAL(clicked()),this,SLOT(accept()));
	QObject::connect(qpb_cancel,SIGNAL(clicked()),this,SLOT(reject()));
}

FiltersForm::~FiltersForm()
{

}
