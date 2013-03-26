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
		//*****Создание и уничтожение экземпляра класса*****
		//
		//Конструктор. В качестве параметров принимает два числа типа int - количество строк и столбцов в таблице
		//
        explicit SyncQueueItemModel(int iCountRow, int iCountColumn,QObject *parent = 0);
		~SyncQueueItemModel(){delete modelData;}
		//**********

		//*****Функции для работы с параметрами строк и столбцов таблицы*****
		//
		//Возврат количества столбцов
        //
		int columnCount(const QModelIndex& parent = QModelIndex()) const;
		//
		//Возврат количества строк потомков для узла
        //
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		//
		//Возврат индекса верхней видимой строки
		//
		int getTopViewRow() const;
		//
		//Возврат индекса нижней видимой строки
		//
		int getBottomViewRow() const;
		//
		//Функция установки видимой области виджета отображения.
		//В случае если полученные индексы ошибочны возвращает false
		//
		bool setWindowIndex(int i_topRowIndex,int i_bottomRowIndex);
		//
		//Функция установки количества столбцов в таблице
		//В случае если количество столбцов неверно возвращает false
		//
		bool setColumn(int i_totalColumn);
		//
		//Функция установки количества строк в таблице
		//В случае если количество столбцов неверно возвращает false
		//
		bool setRow(int i_totalRow);
		//
		//Добавление строки в позицию i_index
		//
		bool insertDataRow(int i_index,const QVector<QString>* i_dataRow);
		//
		//Удаление строки из позиции i_index
		//
		bool deleteDataRow(int i_index);
		//
		//Возврат отображаемого количества строк
		//
		int getViewRowsCount() const;
		//**********

		//*****Функция для получения и обработки данных*****
		//
		//Возврат указателя на данные модели
		//
		const QVector< QString >* const getDataVector() const;
		//
		//Функция передачи данных в модель
		//В случае если указатель на данные 0, возвращает false
		//
		bool setDataVector(QVector< QString >* i_modelData);
		//
		//Переопределенный метод data()
		//
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole ) const;
		//
		//Функция установки флага пустой модели
		//Предыдущее значение флага возвращается
		//
		bool setEmptyData(bool newFlag);
		//
		//Возврат значения флага пустой модели
		//
		bool getEmptyData() const;
		//**********

		//*****Установка заголовков таблицы*****
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		//**********

		//*****Поскольку потомков у узлов нет функция возвращает всегда false*****
		bool hasChildren ( const QModelIndex & parent = QModelIndex() );
		//**********

		//*****Вспомогательные функции*****
		//
		//Функция сброса модели в начальное состояние
		//
		void clear();
		//**********

private:
		//*****Параметры строк и столбцов таблицы*****
        int countRow;      //Общее количество строк
        int countColumn;   //Общее количество столбцов

		int topRowIndex;   //Индекс верхней отображаемой строки
		int bottomRowIndex;//Индекс ниженей отображаемой строки
		//**********

		//*****Флаги отображения данных*****
		bool emptyData;//Флаг отсутствия данных, если true то данные отсутствуют и необходимо вернуть QVariant
					   //если false, то выполняется обработка поступившего запроса на получение данных
		//**********

		//*****Данные хранимые в модели*****
		QVector< QString > * modelData;//Контейнер для храения данных полученных по запросу
		//**********
};

#endif
