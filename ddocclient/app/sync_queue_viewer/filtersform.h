#ifndef FILTERSFORM_H
#define FILTERSFORM_H

#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QGridLayout>

class FiltersForm : public QDialog
{
	Q_OBJECT

public:
	FiltersForm(QWidget *parent);
	~FiltersForm();

private:
	QGroupBox* qgb_organizations;
	QGroupBox* qgb_objectType;
	QGroupBox* qgb_result;

	QCheckBox* qcb_org1;
	QCheckBox* qcb_org2;

	QCheckBox* qcb_type1;
	QCheckBox* qcb_type2;
	QCheckBox* qcb_type3;
	QCheckBox* qcb_type4;
	QCheckBox* qcb_type5;
	QCheckBox* qcb_type6;
	QCheckBox* qcb_type7;
	QCheckBox* qcb_type8;
	QCheckBox* qcb_type9;
	QCheckBox* qcb_type10;

	QCheckBox* qcb_resultType1;
	QCheckBox* qcb_resultType2;
	QCheckBox* qcb_resultType3;
	QCheckBox* qcb_resultType4;

	QPushButton* qpb_ok;
	QPushButton* qpb_cancel;

	QLabel* qlb_from;
	QLabel* qlb_to;

	QDateTimeEdit* qdte_from;
	QDateTimeEdit* qdte_to;
};

#endif // FILTERSFORM_H
