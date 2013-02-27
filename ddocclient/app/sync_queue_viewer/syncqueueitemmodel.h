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

		//Возврат количества столбцов
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
		//Возврат количества строк
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
		//Установка флага перемещения
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
        KKSResult *res;    //Результат запроса

        int countRow;      //Количество строк
        int countColumn;   //Количество столбцов
        int pos_cursor;    //Позиция курсора

        QString cursorName;//Название курсора
        bool move;         //Флаг перемещения

        KKSDatabase * db;  //Указатель на базу данных
};

#endif
