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

		//Возврат количества столбцов
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
		//Возврат количества строк потомков для узла
        int rowCount(const QModelIndex& parent = QModelIndex()) const;

		//*****Установка данных*****
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
		//**********

		//*****Установка заголовков таблицы*****
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		//**********

		//*****Поскольку потомков у узлов нет функция возвращает всегда false*****
		bool hasChildren ( const QModelIndex & parent = QModelIndex() ) {return false;}
		//**********
		
		//
		//Функция передачи данных в модель
		//
		void setDataVector(QVector< QString >* i_modelData){modelData = i_modelData;};
		//
		//Функция установки видимой области виджета отображения
		//
		void setWindowIndex(int input_topRowIndex,int input_bottomRowIndex);
		//
		//Функция установки флага пустой модели
		//
		bool setEmptyData(bool newFlag);

    private:
        int countRow;      //Количество строк
        int countColumn;   //Количество столбцов

		int topRowIndex;   //Индекс верхней отображаемой строки
		int bottomRowIndex;//Индекс ниженей отображаемой строки

		//
		//Флаг пустой модели
		//
		bool emptyData;

		//
		//Контейнер для храения данных
		//
		QVector< QString > * modelData;
};

#endif
