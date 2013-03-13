#ifndef SYNCQUEUEITEMMODEL_H
#define SYNCQUEUEITEMMODEL_H

#include "kksresult.h"

#include <QAbstractItemModel>
#include <QVector>
#include <QStandardItem>

class KKSDatabase;

class SyncQueueItemModel:public QAbstractTableModel
{
    Q_OBJECT

    public:
        explicit SyncQueueItemModel(int iCountRow, int iCountColumn,QObject *parent = 0);
		~SyncQueueItemModel(){delete modelData;}

		//������� ���������� ��������
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
		//������� ���������� ����� �������� ��� ����
        int rowCount(const QModelIndex& parent = QModelIndex()) const;

		//*****��������� ������*****
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
		//**********

		//*****��������� ���������� �������*****
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		//**********

		//*****��������� �������� � ����� ��� ������� ���������� ������ false*****
		bool hasChildren ( const QModelIndex & parent = QModelIndex() ) {return false;}
		//**********
		
		//
		//������� �������� ������ � ������
		//
		void setDataVector(QVector< QString >* i_modelData){modelData = i_modelData;};
		//
		//������� ��������� ������� ������� ������� �����������
		//
		void setWindowIndex(int input_topRowIndex,int input_bottomRowIndex);
		//
		//������� ��������� ����� ������ ������
		//
		bool setEmptyData(bool newFlag);

    private:
        int countRow;      //���������� �����
        int countColumn;   //���������� ��������

		int topRowIndex;   //������ ������� ������������ ������
		int bottomRowIndex;//������ ������� ������������ ������

		//
		//���� ������ ������
		//
		bool emptyData;

		//
		//��������� ��� ������� ������
		//
		QVector< QString > * modelData;
};

#endif
