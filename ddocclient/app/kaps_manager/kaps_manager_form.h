#ifndef KAPS_MANAGER_FORM_H
#define KAPS_MANAGER_FORM_H

#include <QDialog>

class QFile;
class QLabel;
class QLineEdit;
class QProgressDialog;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;

class KKSDatabase;


namespace Ui
{
    class kaps_manager_form;
};


class KapsManagerForm: public QDialog
{
    Q_OBJECT

public:
    KapsManagerForm(KKSDatabase * db, QWidget *parent=0, Qt::WFlags f=0);

private slots:

    void slotKapsChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
    void slotTaskChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
    void slotTaskVarChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
    void slotFileChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);


private:
	
    void initConnections();
    void activateKaps(int idKaps);
    void activateTask(int idTask);
    void activateTaskVar(int idTaskVar);

    void clearTasks();
    void clearTaskVars();
    void clearFiles();

    void loadTasks(int idKaps);
    void loadTaskVars(int idTask);
    void loadFiles(int idTaskVar);

    int getCurrentKaps();
    int getCurrentTask();
    int getCurrentTaskVar();
    int getCurrentFile();

    void initKapsData();

    
    Ui::kaps_manager_form *UI;
	KKSDatabase * db;

};

#endif
