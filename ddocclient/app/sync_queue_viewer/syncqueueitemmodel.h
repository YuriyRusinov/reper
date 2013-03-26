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
		//*****�������� � ����������� ���������� ������*****
		//
		//�����������. � �������� ���������� ��������� ��� ����� ���� int - ���������� ����� � �������� � �������
		//
        explicit SyncQueueItemModel(int iCountRow, int iCountColumn,QObject *parent = 0);
		~SyncQueueItemModel(){delete modelData;}
		//**********

		//*****������� ��� ������ � ����������� ����� � �������� �������*****
		//
		//������� ���������� ��������
        //
		int columnCount(const QModelIndex& parent = QModelIndex()) const;
		//
		//������� ���������� ����� �������� ��� ����
        //
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		//
		//������� ������� ������� ������� ������
		//
		int getTopViewRow() const;
		//
		//������� ������� ������ ������� ������
		//
		int getBottomViewRow() const;
		//
		//������� ��������� ������� ������� ������� �����������.
		//� ������ ���� ���������� ������� �������� ���������� false
		//
		bool setWindowIndex(int i_topRowIndex,int i_bottomRowIndex);
		//
		//������� ��������� ���������� �������� � �������
		//� ������ ���� ���������� �������� ������� ���������� false
		//
		bool setColumn(int i_totalColumn);
		//
		//������� ��������� ���������� ����� � �������
		//� ������ ���� ���������� �������� ������� ���������� false
		//
		bool setRow(int i_totalRow);
		//
		//���������� ������ � ������� i_index
		//
		bool insertDataRow(int i_index,const QVector<QString>* i_dataRow);
		//
		//�������� ������ �� ������� i_index
		//
		bool deleteDataRow(int i_index);
		//
		//������� ������������� ���������� �����
		//
		int getViewRowsCount() const;
		//**********

		//*****������� ��� ��������� � ��������� ������*****
		//
		//������� ��������� �� ������ ������
		//
		const QVector< QString >* const getDataVector() const;
		//
		//������� �������� ������ � ������
		//� ������ ���� ��������� �� ������ 0, ���������� false
		//
		bool setDataVector(QVector< QString >* i_modelData);
		//
		//���������������� ����� data()
		//
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
		//
		//������� ��������� ����� ������ ������
		//���������� �������� ����� ������������
		//
		bool setEmptyData(bool newFlag);
		//
		//������� �������� ����� ������ ������
		//
		bool getEmptyData() const;
		//**********

		//*****��������� ���������� �������*****
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		//**********

		//*****��������� �������� � ����� ��� ������� ���������� ������ false*****
		bool hasChildren ( const QModelIndex & parent = QModelIndex() );
		//**********

		//*****��������������� �������*****
		//
		//������� ������ ������ � ��������� ���������
		//
		void clear();
		//**********

private:
		//*****��������� ����� � �������� �������*****
        int countRow;      //����� ���������� �����
        int countColumn;   //����� ���������� ��������

		int topRowIndex;   //������ ������� ������������ ������
		int bottomRowIndex;//������ ������� ������������ ������
		//**********

		//*****����� ����������� ������*****
		bool emptyData;//���� ���������� ������, ���� true �� ������ ����������� � ���������� ������� QVariant
					   //���� false, �� ����������� ��������� ������������ ������� �� ��������� ������
		//**********

		//*****������ �������� � ������*****
		QVector< QString > * modelData;//��������� ��� ������� ������ ���������� �� �������
		//**********
};

#endif
