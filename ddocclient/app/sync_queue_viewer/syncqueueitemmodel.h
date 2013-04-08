#ifndef SYNCQUEUEITEMMODEL_H
#define SYNCQUEUEITEMMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QBrush>

class SyncQueueItemModel:public QAbstractTableModel
{
    Q_OBJECT

    public:
		//*****�������� � ����������� ���������� ������*****
		//
		//�����������. � �������� ���������� ��������� ��� ����� ���� int - ���������� ����� � �������� � �������
		//
        explicit SyncQueueItemModel(int iCountRow,int iCountColumn,QObject *parent = 0);
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
		bool setWindowIndex(const int i_topRowIndex,const int i_bottomRowIndex);
		//
		//������� ��������� ���������� �������� � �������
		//� ������ ���� ���������� �������� ������� ���������� false
		//
		bool setColumn(const int i_totalColumn);
		//
		//������� ��������� ���������� ����� � �������
		//� ������ ���� ���������� �������� ������� ���������� false
		//
		bool setRow(const int i_totalRow);
		//
		//���������� ������ � ������� i_index
		//������ ������ ����� ������ 0
		//
		int insertDataRow(const int i_index,const QVector<QString>& i_dataRow);
		//
		//�������� ������ �� ������� i_index
		//
		int deleteDataRow(const int i_index);
		//
		//������� ������������� ���������� �����
		//
		int getViewRowsCount() const;
		//**********

		//*****������� ��� ��������� � ��������� ������*****
		//
		//������� �������� ������ � ������
		//� ������ ���� ��������� �� ������ 0, ���������� false
		//
		int setDataVector(const QVector< QString >& i_modelData);
		//
		//���������������� ����� data()
		//
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
		//
		//������� ��������� ����� ������ ������
		//���������� �������� ����� ������������
		//
		void setEmptyData(bool newFlag);
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
		QVector<QString> * modelData;//��������� ��� ������� ������ ���������� �� �������
		//**********

		//
		//������� �������������
		//
		inline void init(int i_row = 0,int i_column = 0);
		//
		//������� �������� ��������� ������ � ������� ����
		//
		inline int visualIndex(int i_index) const;
		//
		//������� ��������� Qt::DecorationRole
		//
		inline QVariant dataDecoration(int i_index) const;
		//
		//������� ��������� Qt::BackgroundRole
		//
		inline QVariant dataBackground(int i_index) const;
		//
		//������� ��������� Qt::DisplayRole
		//
		inline QVariant dataDisplay(int i_index,int i_indexColumn) const;
};

#endif
