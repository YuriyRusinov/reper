#include "./UserInterface/DataStream/DataStream.h"

dataStream::dataStream(KKSDatabase* adb,QObject *parent):
    QObject(parent), flagDBisOpen(false)
{
    db = adb;
    res = 0;

    initQuery();
    openDB();
}

dataStream::~dataStream()
{
    closeDB();
}

void dataStream::initQuery()
{
    sqlQuery = QString("\
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
                                \"is_main\", \
                                \"is_created\", \
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
                                organization.\"is_main\", \
                                organization.\"is_created\", \
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
                                organization.\"is_main\", \
                                organization.\"is_created\", \
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
                                ot1.port, \
                                organization_rec.\"is_main\", \
                                organization_rec.\"is_created\", \
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

void dataStream::openDB()
{
    if(flagDBisOpen)
        return;

    db->begin();
    flagDBisOpen = true;
}

void dataStream::closeDB()
{
    if(!flagDBisOpen)
        return;

    db->commit();
    flagDBisOpen = false;
}

const QList<dyndoc_mainStructs::hostModelInf>& dataStream::getData()
{
    organizations.clear();

    executeQuery();
    createOrganizationsListFromRes();

    return organizations;
}

void dataStream::executeQuery()
{
    if(!( res = db->execute(sqlQuery) ))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Data base error"));
        msgBox.setText("Res is NULL!");
        msgBox.exec();
    }
}

void dataStream::createOrganizationsListFromRes()
{
    if(!res)
        return;

    for(int i = 0 ; i < res->getRowCount() ; i++)
    {
        dyndoc_mainStructs::hostModelInf newOrg;

        newOrg.structInf.id = res->getCellAsString(i,1);
        newOrg.structInf.name = res->getCellAsString(i,3);
        newOrg.structInf.parentName = res->getCellAsString(i,4);
        newOrg.structInf.protocol = "TCP/IP";
        newOrg.address.address = res->getCellAsString(i,33);
        newOrg.address.port = res->getCellAsInt(i,34);//getCellAsString(i,34);

        organizations.append(newOrg);
    }

    delete res;
}

dyndoc_mainStructs::dbInf dataStream::getDbInf() const
{
    dyndoc_mainStructs::dbInf dbOutInf;

    dbOutInf.dbName = db->getName();
    dbOutInf.userName = db->getUser();
    dbOutInf.password = db->getPass();
    dbOutInf.hostAddress = db->getHost();
    dbOutInf.port = db->getPort().toInt();

    return dbOutInf;
}
