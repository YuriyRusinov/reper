#ifndef KAPS_MANAGER_FORM_H
#define KAPS_MANAGER_FORM_H

#include <QDialog>

class QFile;
class QDate;
class QDateTime;
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

    void addTask();
    void editTask();
    void delTask();

    void addTaskVar();
    void editTaskVar();
    void delTaskVar();

    void addFile();
    void editFile();
    void delFile();

    void getFileFromDD();

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

    int updateTaskInDB(int id, int idKaps, const QString & name, const QString & desc, const QString & folder);
    int updateTaskVarInDB(int id, int idTask, const QString & name, const QString & desc, const QString & folder, const QString & author, const QString & region, const QDate & date);
    int updateFileInDB(int id, 
                       int idTaskVar, 
                       int idType, 
                       const QString & name,
                       const QString & fileName,
                       const QString & url, 
                       const QString & ka, 
                       const QString & region, 
                       const QDateTime & timeShoot,
                       int * idF); //11111
    int deleteTaskInDB(int idTask);
    int deleteTaskVarInDB(int idTaskVar);
    int deleteFileInDB(int idFile);

    int addFileToDD(const QString & url, const QString & name, int idFile);
    int rInsertUrl(const QString & fileName, int fileType, const QString & fileExt);
    int createFileInDD(int idUrl, const QString & localUrl);

    int downloadFile(int idF, const QString & localUrl);
    int getFileForItem(QTreeWidgetItem * item);//22222
    QString getFileFolder(int idType);
    QString getCurrentKapsFolder();
    QString getCurrentTaskFolder();
    QString getCurrentTaskVarFolder();
    QString getRootFolder();


    void initKapsData();

    
    Ui::kaps_manager_form *UI;
	KKSDatabase * db;

};

#endif
