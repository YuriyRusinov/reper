#include "filtersform.h"

FiltersForm::FiltersForm(QWidget *parent)
	: QDialog(parent)
{
    createInterface();
    createConnections();
}

void FiltersForm::createInterface()
{
    setModal(true);
    setWindowTitle(tr("Data base view: filters"));

    qgb_organizations = new QListView(this);
    qgb_objectType = new QListView(this);
    qgb_result = new QListView(this);

    QStringList sl = QStringList();

    sl << tr("OD 1") << tr("OD 2");

    model_org = new QStringListModel(sl,this);
    qgb_organizations->setModel(model_org);
    qgb_organizations->setSelectionMode(QAbstractItemView::MultiSelection);
    select_org = qgb_organizations->selectionModel();

    sl.clear();
    sl << tr("Category(system)") << tr("IO(system)") << tr("IOR(directory record)")
       << tr("IOR(record additional table directory)") << tr("Additional table directory")
       << tr("Transfer request to the ping") << tr("Reserved")
       << tr("Transfer complete directory of organizations to the new object")
       << tr("Transfer directory entries for all organizations involved in the exchange of information objects") << tr("Public DL");

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

    qpb_ok = new QPushButton(tr("Ok"),this);
    qpb_cancel = new QPushButton(tr("Cancel"),this);

    qcb_timeTo = new QCheckBox();
    qcb_timeTo->setCheckState(Qt::Checked);
    qcb_timeFrom = new QCheckBox();
    qcb_timeFrom->setCheckState(Qt::Checked);

    qlb_from = new QLabel(tr("From:"),this);
    qlb_to = new QLabel(tr("To:"),this);

    qdte_from = new QDateTimeEdit(this);
    qdte_to = new QDateTimeEdit(this);

    qdte_from->setDisplayFormat("dd.MM.yyyy hh:mm:ss");
    qdte_to->setDisplayFormat("dd.MM.yyyy hh:mm:ss");

    qdte_to->setDate(QDate().currentDate());
    qdte_to->setTime(QTime().currentTime());

    QGridLayout* qgl_date = new QGridLayout;
    qgl_date->addWidget(qcb_timeFrom,0,0,Qt::AlignRight);
    qgl_date->addWidget(qcb_timeTo,1,0,Qt::AlignRight);
    qgl_date->addWidget(qlb_from,0,1,Qt::AlignRight);
    qgl_date->addWidget(qlb_to,1,1,Qt::AlignRight);
    qgl_date->addWidget(qdte_from,0,2,Qt::AlignRight);
    qgl_date->addWidget(qdte_to,1,2,Qt::AlignRight);

    QGridLayout* qgl_groopbox = new QGridLayout;
    QHBoxLayout* qhbl_buttons = new QHBoxLayout;
    QVBoxLayout* qvbl_main = new QVBoxLayout;

    qgl_groopbox->addWidget(qlb_organizations,0,0,Qt::AlignHCenter);
    qgl_groopbox->addWidget(qlb_objectType,0,1,Qt::AlignHCenter);
    qgl_groopbox->addWidget(qlb_result,0,2,Qt::AlignHCenter);
    qgl_groopbox->addWidget(qgb_organizations,1,0,Qt::AlignHCenter);
    qgl_groopbox->addWidget(qgb_objectType,1,1,Qt::AlignHCenter);
    qgl_groopbox->addWidget(qgb_result,1,2,Qt::AlignHCenter);

    QSplitter* qsh_datebutton = new QSplitter(Qt::Horizontal,this);

    qhbl_buttons->addLayout(qgl_date);
    qhbl_buttons->addWidget(qsh_datebutton);
    qhbl_buttons->addWidget(qpb_ok);
    qhbl_buttons->addWidget(qpb_cancel);

    qvbl_main->addLayout(qgl_groopbox);
    qvbl_main->addLayout(qhbl_buttons);
    setLayout(qvbl_main);
}

void FiltersForm::createConnections()
{
    QObject::connect(qpb_ok,SIGNAL(clicked()),this,SLOT(accept()));
    QObject::connect(qpb_cancel,SIGNAL(clicked()),this,SLOT(reject()));
    QObject::connect(qcb_timeFrom,SIGNAL(clicked()),this,SLOT(slot_timeCheck()));
    QObject::connect(qcb_timeTo,SIGNAL(clicked()),this,SLOT(slot_timeCheck()));
}

FiltersForm::~FiltersForm()
{
	delete qlb_organizations;
	delete qlb_objectType;
	delete qlb_result;

	delete qgb_organizations;
	delete qgb_objectType;
	delete qgb_result;

	delete model_org;
	select_org = 0;

	delete model_obj;
	select_obj = 0;

	delete model_res;
	select_res = 0;

	delete qcb_timeTo;
	delete qcb_timeFrom;

	delete qpb_ok;
	delete qpb_cancel;

	delete qlb_from;
	delete qlb_to;

	delete qdte_from;
	delete qdte_to;
}

QStringList FiltersForm::getOrganization()
{
    return selectionList(*select_org);
}

QStringList FiltersForm::getObjectType()
{
    return selectionList(*select_obj);
}

QStringList FiltersForm::getResult()
{
    return selectionList(*select_res);
}

inline QStringList FiltersForm::selectionList(const QItemSelectionModel& selection)
{
    QStringList resList;
    QModelIndex index;

    foreach (index,selection.selectedRows())
    {
        resList<<QString().setNum(index.row()+1);
    }

    return resList;
}

QString FiltersForm::getDateFrom()
{
    return dateInFormat(*qdte_from);
}

QString FiltersForm::getDateTo()
{
    return dateInFormat(*qdte_to);
}

inline QString FiltersForm::dateInFormat(const QDateTimeEdit& date)
{
    QString res;

    if(date.isEnabled())
    {
        res = date.dateTime().toString("dd.MM.yyyy hh:mm:ss");
    }

    return res;
}

//**********

void FiltersForm::slot_timeCheck()
{
	if(!qcb_timeFrom->checkState())
	{
		qdte_from->setEnabled(false);
	}
	else
	{
		qdte_from->setEnabled(true);
	}

	if(!qcb_timeTo->checkState())
	{
		qdte_to->setEnabled(false);
	}
	else
	{
		qdte_to->setEnabled(true);
	}
}
