#ifndef DYNDOC_SYNCH_FORM_H
#define DYNDOC_SYNCH_FORM_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>

#include "kksdatabase.h"

#include "dyndoc_synch.h"
#include "dyndoc_itemmodel.h"

class dyndoc_itemModel;

namespace Ui {
class dyndoc_synch_form;
}

class dyndoc_synch_form : public QDialog
{
    Q_OBJECT
    
public:
    explicit dyndoc_synch_form(KKSDatabase* adb,QWidget *parent = 0);
    ~dyndoc_synch_form();

    
private:
    Ui::dyndoc_synch_form *ui;

    //
    //������ ������ � �������������
    //
    dyndocTreeView*   ddTreeView;
    dyndoc_itemModel* ddItemModel;

    //
    //�����
    //
    bool flag_DatabaseOpen;//���� �������� ���� ������

    //
    //���������� ��� �������� �������
    //
    QString sqlCursor;
    //
    //��������� �� ���� ������
    //
    KKSDatabase * db;
    //
    //��������� �� ��������� �������
    //
    KKSResult* res;

    //
    //����������� ������
    //
    dyndoc_synch_form(dyndoc_synch_form& adb);                       //������ �� �������� ������������ �����������
    const dyndoc_synch_form& operator=(const dyndoc_synch_form& rhs);//������ �� �������� ��������� ������������

    //
    //������� ������������� ������
    //
    inline void init();            //������� ������������� ������ ������
    inline void init_slots() const;//������� ����������� �������� � ������
    inline void init_interface();  //������� ��������� ����������

    inline void init_query();      //������� ������������� �������

    //
    //������� ������ � ��
    //
    int open_database(); //������� �������� �������
    int close_database();//������� �������� �������
    int databaseData();  //������� ��������� ������

    //
    //������� ���������� ��������� ������
    //
    int addChilds(QStandardItem& rhs,const QString& text,int& lastFindChildPosition);

    //
    //*****������� ���������� ������ � ��������*****
    //
    //������� ���������� ������� � ��������
    //
    int addColumns(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� �����
    //
    inline int addColumnName(QList<QStandardItem*>& rhs,int query_index);
    //
    //������ ���������� ������� ��������� � ��������
    //
    inline int addColumnProtocol(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� ������
    //
    inline int addColumnAdress(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� �����
    //
    inline int addColumnPort(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� �������������
    //
    inline int addColumnSynch(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� ���� ������
    //
    inline int addColumnDb(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� ������������� �������������
    //
    inline int addColumnInit(QList<QStandardItem*>& rhs,int query_index);
    //
    //������� ���������� ������� �������� ��������
    //
    inline int addColumnTrans(QList<QStandardItem*>& rhs,int query_index);
    //**********
};

#endif // DYNDOC_SYNCH_FORM_H
