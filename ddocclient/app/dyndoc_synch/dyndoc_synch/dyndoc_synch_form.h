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
    //Данные модели и представления
    //
    dyndocTreeView*   ddTreeView;
    dyndoc_itemModel* ddItemModel;

    //
    //Флаги
    //
    bool flag_DatabaseOpen;//Флаг открытой базы данных

    //
    //Переменная для хранения запроса
    //
    QString sqlCursor;
    //
    //Указатель на базу данных
    //
    KKSDatabase * db;
    //
    //Указатель на результат запроса
    //
    KKSResult* res;

    //
    //Запрещенные вызовы
    //
    dyndoc_synch_form(dyndoc_synch_form& adb);                       //Запрет на создание конструктора копирования
    const dyndoc_synch_form& operator=(const dyndoc_synch_form& rhs);//Запрет на создание оператора присваивания

    //
    //Функции инициализации класса
    //
    inline void init();            //Функция инициализации данных класса
    inline void init_slots() const;//Функция подключения сигналов и слотов
    inline void init_interface();  //Функция настройки интерфейса

    inline void init_query();      //Функция инициализации запроса

    //
    //Функции работы с БД
    //
    int open_database(); //Функция создания курсора
    int close_database();//Функция закрытия курсора
    int databaseData();  //Функция получения данных

    //
    //Функция добавления элементов списка
    //
    int addChilds(QStandardItem& rhs,const QString& text,int& lastFindChildPosition);

    //
    //*****Функции добавления данных в элементы*****
    //
    //Функция добавления колонок к элементу
    //
    int addColumns(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки имени
    //
    inline int addColumnName(QList<QStandardItem*>& rhs,int query_index);
    //
    //Фунция добавления колонки протокола к элементу
    //
    inline int addColumnProtocol(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки адреса
    //
    inline int addColumnAdress(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки порта
    //
    inline int addColumnPort(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки синхронизации
    //
    inline int addColumnSynch(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки базы данных
    //
    inline int addColumnDb(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки первоначльной синхронизации
    //
    inline int addColumnInit(QList<QStandardItem*>& rhs,int query_index);
    //
    //Функция добавления колонки процесса передачи
    //
    inline int addColumnTrans(QList<QStandardItem*>& rhs,int query_index);
    //**********
};

#endif // DYNDOC_SYNCH_FORM_H
