#ifndef SYNCQUEUEITEMMODEL_H
#define SYNCQUEUEITEMMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QBrush>

#include "dataelement.h"
#include "sqv_datastream.h"

class SyncQueueItemModel:public QAbstractTableModel
{
    Q_OBJECT

    public:
        explicit SyncQueueItemModel(int iCountRow,int iCountColumn,QObject *parent = 0);
        ~SyncQueueItemModel(){delete l_modelData;}

		int columnCount(const QModelIndex& parent = QModelIndex()) const;
        int rowCount(const QModelIndex& parent = QModelIndex()) const;

        int getTopViewRow() const;
        int getBottomViewRow() const;

		bool setWindowIndex(const int i_topRowIndex,const int i_bottomRowIndex);
		bool setColumn(const int i_totalColumn);
		bool setRow(const int i_totalRow);

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;

		void setEmptyData(bool newFlag);
        bool getEmptyData() const;

        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        bool hasChildren( const QModelIndex & parent = QModelIndex() );

    public slots:
        void slot_setData(sqv_data::modifyModelData rhs);
        void slot_newWindow(sqv_data::scrollWindow rhs);

    signals:
        void signal_scrollScreen(sqv_data::scrollWindow rhs);

    private:
        int countColumn;

        sqv_data::viewWindowOptions window;

        bool emptyData;

        QList<DataElement>* l_modelData;

        inline void init(int i_row = 0,int i_column = 0);

        void copyDataList(const QList<DataElement>& i_modelData);
        void addRowsForward(const QList<DataElement>& i_modelData);
        void addRowsBackward(const QList<DataElement>& i_modelData);

        inline bool visualRow(int rhs) const;

		inline QVariant dataDecoration(int i_index) const;
		inline QVariant dataBackground(int i_index) const;
		inline QVariant dataDisplay(int i_index,int i_indexColumn) const;

        inline QString returnData(int row_index,int rhs) const;
};

#endif
