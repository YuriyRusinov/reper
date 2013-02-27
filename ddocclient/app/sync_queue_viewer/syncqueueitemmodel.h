#ifndef SYNCQUEUEITEMMODEL_H
#define SYNCQUEUEITEMMODEL_H

#include "kksresult.h"

#include <QAbstractItemModel>
//#include <QStandardItemModel>

class KKSDatabase;

class SyncQueueItemModel:public QAbstractTableModel
{
    Q_OBJECT

    public:
        explicit SyncQueueItemModel(int iCountRow, int iCountColumn, KKSDatabase * adb, QString cursorName = QString(),QObject *parent = 0);
        ~SyncQueueItemModel(){}

		//������� ���������� ��������
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
		//������� ���������� �����
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
		//��������� ����� �����������
        void SetMove(bool amove){move = amove;}

		//***** *****
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
		//**********

		//***** *****
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		//**********

		//***** *****
        void SetCursor(KKSResult* ares, int apos_cursor = 0 ){res = ares;pos_cursor = apos_cursor;}
		//**********

    private:
        KKSResult *res;    //��������� �������

        int countRow;      //���������� �����
        int countColumn;   //���������� ��������
        int pos_cursor;    //������� �������

        QString cursorName;//�������� �������
        bool move;         //���� �����������

        KKSDatabase * db;  //��������� �� ���� ������
};

#endif
