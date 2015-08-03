\encoding win1251

begin;

alter table io_urls add column src_ext varchar;

create table categories_rubrics (
id                   SERIAL               not null,
id_parent            INT4                 null,
id_category          INT4                 null,
name                 VARCHAR              not null,
constraint PK_CATEGORIES_RUBRICS primary key (id)
);

alter table categories_rubrics
   add constraint FK_CATEGORI_REFERENCE_CATEGORI foreign key (id_parent)
      references categories_rubrics (id)
      on delete restrict on update restrict;

alter table categories_rubrics
   add constraint FK_CATEGORI_REFERENCE_IO_CATEG foreign key (id_category)
      references io_categories (id)
      on delete restrict on update restrict;



select createTempTables();


--
create or replace function aa11() returns int4 as'
declare
    idUnit int4;
    idPosition int4;
begin

    select getNextSeq(\'units\', \'id\') into idUnit;
    select getNextSeq(\'positions\', \'id\') into idPosition;

    insert into positions (id, name, code, is_boss) values (idPosition, \'Сотрудник\', \'СОТРУДНИК\', false);

    insert into units  (id, id_executor, name, group_name, code) values(idUnit, NULL, \'Администратор ККССИТО\', \'admin\', \'ADMINGROUP\');
    insert into dls (id_position, id_unit, id_user, is_io, name) values(idPosition, idUnit, 1, false, \'Администратор\');
    
    return 1;

end
'language plpgsql;


select aa11();

drop function aa11();


alter table io_objects add column is_completed bool;
ALTER TABLE IO_OBJECTS alter column is_completed set default FALSE;
update io_objects set is_completed = 'FALSE' where is_completed IS NULL;
ALTER TABLE IO_OBJECTS alter column is_completed set not null;


insert into attrs_categories (id_io_category, id_io_attribute, def_value, is_mandatory, is_read_only) values (19, 21, NULL, true, false);

drop function rgetabsurl(int4);
drop function rinserturl(varchar, varchar, int4);

\i ./functions/readd_functions.sql

\i ./initprivs.sql

commit;
