#ifndef FILTERSFORM_H
#define FILTERSFORM_H

#include <QDialog>
#include <QPushButton>
#include <QSplitter>
#include <QCheckBox>
#include <QDateTimeEdit>
#include <QLabel>
#include <QGridLayout>
#include <QListView>
#include <QStringListModel>

class QItemSelectionModel;

class FiltersForm : public QDialog
{
	Q_OBJECT

public:
	FiltersForm(QWidget *parent);
	~FiltersForm();

	QStringList getOrganization();
	QStringList getObjectType();
	QStringList getResult();
	QString getDateFrom();
	QString getDateTo();

private slots:
	void slot_timeCheck();

private:
	QLabel* qlb_organizations;
	QLabel* qlb_objectType;
	QLabel* qlb_result;

	QListView* qgb_organizations;
	QListView* qgb_objectType;
	QListView* qgb_result;

	QStringListModel* model_org;
	QItemSelectionModel* select_org;

	QStringListModel* model_obj;
	QItemSelectionModel* select_obj;

	QStringListModel* model_res;
	QItemSelectionModel* select_res;

	QCheckBox* qcb_timeTo;
	QCheckBox* qcb_timeFrom;

	QPushButton* qpb_ok;
	QPushButton* qpb_cancel;

	QLabel* qlb_from;
	QLabel* qlb_to;

	QDateTimeEdit* qdte_from;
	QDateTimeEdit* qdte_to;

    inline void createInterface();
    inline void createConnections();

    inline QStringList selectionList(const QItemSelectionModel& selection);
    inline QString dateInFormat(const QDateTimeEdit& date);
};

#endif // FILTERSFORM_H
