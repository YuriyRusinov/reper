#include "dyndoc_synch_form.h"
#include "ui_dyndoc_synch_form.h"

dyndoc_synch_form::dyndoc_synch_form(KKSDatabase *adb, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dyndoc_synch_form),res()
{
    ui->setupUi(this);

    db = adb;

    init();
    init_slots();
    init_interface();

    init_query();

    if(!open_database())
        databaseData();
}

dyndoc_synch_form::~dyndoc_synch_form()
{   
    delete ddTreeView;

    delete ui;
}

//*****************************************************************************
//
//Функции инициализации класса
//

//Функция инициализации данных класса
void dyndoc_synch_form::init()
{
    QVBoxLayout* frameLayout = new QVBoxLayout;
    ddTreeView  = new dyndocTreeView(ui->frame);
    ddItemModel = new dyndoc_itemModel(ui->frame);

    flag_DatabaseOpen = false;

    ddTreeView->setModel(ddItemModel);
    ddItemModel->setColumnCount(8);

    frameLayout->addWidget(ddTreeView);
    ui->frame->setLayout(frameLayout);
}
//Функция подключения сигналов и слотов
void dyndoc_synch_form::init_slots() const
{
    //Соединение для кнопки выхода
    connect(ui->qpb_exit,SIGNAL(clicked()),this,SLOT(close()));

    //connect(ddItemModel,SIGNAL(rowsInserted(QModelIndex,int,int)),ddTreeView,SLOT(edit(QModelIndex)));
}
//Функция настройки интерфейса
void dyndoc_synch_form::init_interface()
{
    //Установка наименований столбцов
    ddItemModel->setHeaderData(0,Qt::Horizontal,QString(tr("Organization")));
    ddItemModel->setHeaderData(0,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(1,Qt::Horizontal,QString(tr("Protocol")));
    ddItemModel->setHeaderData(1,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(2,Qt::Horizontal,QString(tr("Adress")));
    ddItemModel->setHeaderData(2,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(3,Qt::Horizontal,QString(tr("Port")));
    ddItemModel->setHeaderData(3,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(4,Qt::Horizontal,QString(tr("Syncronizer\nactivity")));
    ddItemModel->setHeaderData(4,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(5,Qt::Horizontal,QString(tr("Data base\nactivity")));
    ddItemModel->setHeaderData(5,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(6,Qt::Horizontal,QString(tr("Initial\nsyncronization")));
    ddItemModel->setHeaderData(6,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    ddItemModel->setHeaderData(7,Qt::Horizontal,QString(tr("Transmition")));
    ddItemModel->setHeaderData(7,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
}
//Функция инициализации запроса к базе данных
void dyndoc_synch_form::init_query()
{
    sqlCursor = QString("\
                        with recursive organization_rec \
                        (       id,\
                                unique_id ,\             \
                                \"name\",\
                                \"short_name\",\
                                \"id_parent\",\
                                \"id_curr_mode\",\
                                \"id_boss\",\
                                \"code_name\",\
                                \"tree_symbol\",\
                                \"id_type_org\",\
                                \"id_parent1\",\
                                \"id_substitutor\",\
                                \"id_prev_mode\",\
                                \"email_prefix\",\
                                \"latitude\",\
                                \"longitude\",\
                                \"map_symbol\",\
                                \"dt_prev_mode\",\
                                \"dt_curr_mode\",\
                                ii_rec_order)\
                        as\
                            (select\
                                distinct organization.id,\
                                organization.unique_id,\
                                organization.\"name\",\
                                organization.\"short_name\",\
                                organization.\"id_parent\",\
                                organization.\"id_curr_mode\",\
                                organization.\"id_boss\",\
                                organization.\"code_name\",\
                                organization.\"tree_symbol\",\
                                organization.\"id_type_org\",\
                                organization.\"id_parent1\",\
                                organization.\"id_substitutor\",\
                                organization.\"id_prev_mode\",\
                                organization.\"email_prefix\",\
                                organization.\"latitude\",\
                                organization.\"longitude\",\
                                organization.\"map_symbol\",\
                                organization.\"dt_prev_mode\",\
                                organization.\"dt_curr_mode\",\
                                0\
                            from\
                                organization\
                            where\
                                organization.\"id_parent\" isnull\
                        \
                        UNION ALL\
                            select\
                                organization.id,\
                                organization.unique_id,\
                                organization.\"name\",\
                                organization.\"short_name\",\
                                organization.\"id_parent\",\
                                organization.\"id_curr_mode\",\
                                organization.\"id_boss\",\
                                organization.\"code_name\",\
                                organization.\"tree_symbol\",\
                                organization.\"id_type_org\",\
                                organization.\"id_parent1\",\
                                organization.\"id_substitutor\",\
                                organization.\"id_prev_mode\",\
                                organization.\"email_prefix\",\
                                organization.\"latitude\",\
                                organization.\"longitude\",\
                                organization.\"map_symbol\",\
                                organization.\"dt_prev_mode\",\
                                organization.\"dt_curr_mode\",\
                                organization_rec.ii_rec_order+1\
                            from\
                                organization_rec,\
                                organization\
                            where\
                                organization_rec.id = organization.\"id_parent\"\
                            )\
                            \
                            select\
                                distinct organization_rec.id,\
                                organization_rec.unique_id ,\
                                organization_rec.\"name\",\
                                organization_rec.\"short_name\",\
                                case when organization_rec.\"id_parent\" isnull then NULL else organization1.\"name\" end as \"id_parent\",\
                                organization_rec.\"id_parent\" as \"id_parent_fk\",\
                                case when organization_rec.\"id_parent\" isnull then NULL else organization1.unique_id end as \"id_parent_uid\",\
                                work_mode.\"name\" as \"id_curr_mode\",\
                                organization_rec.\"id_curr_mode\" as \"id_curr_mode_fk\",\
                                work_mode.unique_id as \"id_curr_mode_uid\",\
                                case when organization_rec.\"id_boss\" isnull then NULL else position.\"name\" end as \"id_boss\",\
                                organization_rec.\"id_boss\" as \"id_boss_fk\",\
                                case when organization_rec.\"id_boss\" isnull then NULL else position.unique_id end as \"id_boss_uid\",\
                                organization_rec.\"code_name\",\
                                organization_rec.\"tree_symbol\",\
                                organization_type.\"name\" as \"id_type_org\",\
                                organization_rec.\"id_type_org\" as \"id_type_org_fk\",\
                                organization_type.unique_id as \"id_type_org_uid\",\
                                case when organization_rec.\"id_parent1\" isnull then NULL else organization2.\"name\" end as \"id_parent1\",\
                                organization_rec.\"id_parent1\" as \"id_parent1_fk\",\
                                case when organization_rec.\"id_parent1\" isnull then NULL else organization2.unique_id end as \"id_parent1_uid\",\
                                case when organization_rec.\"id_substitutor\" isnull then NULL else organization3.\"name\" end as \"id_substitutor\",\
                                organization_rec.\"id_substitutor\" as \"id_substitutor_fk\",\
                                case when organization_rec.\"id_substitutor\" isnull then NULL else organization3.unique_id end as \"id_substitutor_uid\",\
                                work_mode1.\"name\" as \"id_prev_mode\",\
                                organization_rec.\"id_prev_mode\" as \"id_prev_mode_fk\",\
                                work_mode1.unique_id as \"id_prev_mode_uid\",\
                                organization_rec.\"email_prefix\",\
                                organization_rec.\"latitude\",\
                                organization_rec.\"longitude\",\
                                organization_rec.\"map_symbol\",\
                                organization_rec.\"dt_prev_mode\",\
                                organization_rec.\"dt_curr_mode\",\
                                ot1.address,\
                                ot1.port,\
                                organization_rec.ii_rec_order\
                            \
                            from\
                                organization_rec\
                                left join organization organization1 on organization_rec.\"id_parent\" = organization1.id\
                                left join work_mode on organization_rec.\"id_curr_mode\" = work_mode.id\
                                left join position on organization_rec.\"id_boss\" = position.id\
                                left join organization_type on organization_rec.\"id_type_org\" = organization_type.id\
                                left join organization organization2 on organization_rec.\"id_parent1\" = organization2.id\
                                left join organization organization3 on organization_rec.\"id_substitutor\" = organization3.id\
                                left join work_mode work_mode1 on organization_rec.\"id_prev_mode\" = work_mode1.id\
                                left join organization_transport ot1 on (organization_rec.id = ot1.id_organization)\
                                left join transport t1 on (ot1.id_transport = t1.id)\
                            \
                            where\
                                1=1\
                            \
                            order by organization_rec.ii_rec_order");
}
//*****************************************************************************

//*****************************************************************************
//
//Функции работы с БД
//

//Функция создания курсора
int dyndoc_synch_form::open_database()
{
    //
    //Проверка существования курсора, в случае если истина - закрытие
    //
    if(flag_DatabaseOpen)
    {
        close_database();
    }

    //
    //Указатель на результат запроса
    //
    res = 0;

    //
    //Открытие базы данных
    //
    db->begin();
    //
    //Выполнение запроса
    //
    res = db->execute(sqlCursor);

    if(!res)
    {
        db->commit();
        return 10;
    }

    //
    //Установка флага открытой БД в положение открыт
    //
    flag_DatabaseOpen = true;

    return 0;
}
//Функция закрытия курсора
int dyndoc_synch_form::close_database()
{
    db->close("sync_cursor");
    flag_DatabaseOpen = false;

    return 0;
}
//Функция получения данных
int dyndoc_synch_form::databaseData()
{
    QStandardItem* root_item = ddItemModel->invisibleRootItem();

    int lastFindChildPosition = 0;

    addChilds(*root_item,QString(""),lastFindChildPosition);

    return 0;
}
//*****************************************************************************

//
//Функция добавления элементов списка
//
//1. Установка индекса потомка в 0
//2. Для всех строк в запросе
//    2.1. Если наименование элемента соответствует названию родителя в строке
//        2.1.1. Создаем новый объект QStandartItem
//        2.1.2. Записываем в него данные
//        2.1.3. Добавляем объект к родителю в качестве строки
//        2.1.4. Увеличиваем индекс потомков на 1
//3. Для всех потомков запустить рекурсивно функцию
//
int dyndoc_synch_form::addChilds(QStandardItem& rhs,const QString& text,int& lastFindChildPosition)
{    
    int col = 0;//Индекс потомков элемента

    //
    //Обход всех строк в запросе
    //
    for(int i=lastFindChildPosition;i<res->getRowCount();i++)
    {
        if(text == res->getCellAsString(i,4))
        {
            QList<QStandardItem*>* row_itemList = new QList<QStandardItem*>;
            addColumns(*row_itemList,i);
            rhs.appendRow(*row_itemList);
            col++;
            lastFindChildPosition++;
        }
    }

    //
    //Рекурсивный вызов функции для потомков
    //
    if(rhs.rowCount())//Проверка условия наличия потомков
    {
        for(int i=0;i<rhs.rowCount();i++)
        {
            addChilds(*rhs.child(i),rhs.child(i)->text(),lastFindChildPosition);
        }
    }

    return 0;
}

//
//Функция добавления колонок к элементу
//
int dyndoc_synch_form::addColumns(QList<QStandardItem*>& rhs,int query_index)
{
    addColumnName(rhs,query_index);
    addColumnProtocol(rhs,query_index);
    addColumnAdress(rhs,query_index);
    addColumnPort(rhs,query_index);
    addColumnSynch(rhs,query_index);
    addColumnDb(rhs,query_index);
    addColumnInit(rhs,query_index);
    addColumnTrans(rhs,query_index);

    return 0;
}
//
//Функция добавления колонки имени
//
int dyndoc_synch_form::addColumnName(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(res->getCellAsString(query_index,3),Qt::DisplayRole);
    item->setData(res->getCellAsString(query_index,3),Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
//
//Фунция добавления колонки протокола к элементу
//
int dyndoc_synch_form::addColumnProtocol(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData("TCP/IP",Qt::DisplayRole);
    item->setData("TCP/IP",Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
//
//Функция добавления колонки адреса
//
int dyndoc_synch_form::addColumnAdress(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(res->getCellAsString(query_index,33),Qt::DisplayRole);
    item->setData(res->getCellAsString(query_index,33),Qt::ToolTipRole);

    if(item->text() == "" || item->text() == "localhost")
    {
        QFont boldItemFont = rhs.first()->font();
        boldItemFont.setBold(true);
        rhs.first()->setFont(boldItemFont);
    }

    rhs.append(item);

    return 0;
}
//
//Функция добавления колонки порта
//
int dyndoc_synch_form::addColumnPort(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(res->getCellAsString(query_index,34),Qt::DisplayRole);
    item->setData(res->getCellAsString(query_index,34),Qt::ToolTipRole);

    rhs.append(item);
    return 0;
}
//
//Функция добавления колонки синхронизации
//
int dyndoc_synch_form::addColumnSynch(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Checked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    item->setData("true",Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
//
//Функция добавления колонки базы данных
//
int dyndoc_synch_form::addColumnDb(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Checked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    item->setData("true",Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
//
//Функция добавления колонки первоначльной синхронизации
//
int dyndoc_synch_form::addColumnInit(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Checked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    item->setData("true",Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
//
//Функция добавления колонки процесса передачи
//
int dyndoc_synch_form::addColumnTrans(QList<QStandardItem*>& rhs,int query_index)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Checked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
    item->setData("true",Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
