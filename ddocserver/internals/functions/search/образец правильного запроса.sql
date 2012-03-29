/*select * from ioGetObjectAttrsAll(301)
--select * from ioGetObjectAttrs(301)
--select * from ioGetObjectAttrs(302)
--select acGetAttrCategoryId(168, 25)
select ioCatAttrExist(301, 2030)
select ioAttrExist(301, 25)

select ioUpdateAttr(301, 25, 'sss')

delete from attrs_values where id_attr_category = 2030
 select * from ioSearch('22233a', 237, 'LIKE')
 select at.code from a_types at inner join attributes a on (at.id=a.id_a_type and a.id=25)
 select is_Float8('22233');
 select av.id_io_object from attributes a inner join attrs_categories ac on (ac.id_io_attribute = a.id and a.id = 237 ) inner join attrs_values av on ( av.value LIKE '22233' and av.id_attr_category = ac.id)
*/
 --select * from search_templates

 --select * from generateSearchQuery(13, 'io_objects', 2)
 --select distinct io_objects.id, io_objects.ref_table_name, case when io_objects.id_search_template is null then NULL else search_templates.name end as id_search_template, io_objects.record_text_color, io_objects.record_fill_color, io_objects.is_global, organization.name as id_owner_org, io_sync_types.name as id_sync_type, maclabels.name as id_maclabel, io_states.name as id_io_state, io_objects.insert_time, io_objects.is_system, io_objects.information, io_objects.table_name, case when io_objects.author is null then NULL else users.fio end as author, io_categories.name as id_io_category, io_types.name as id_io_type, io_objects.description, io_objects.name from io_objects  left join search_templates on io_objects.id_search_template = search_templates.id  left join organization on io_objects.id_owner_org = organization.id  left join io_sync_types on io_objects.id_sync_type = io_sync_types.id  left join maclabels on io_objects.id_maclabel = maclabels.id  left join io_states on io_objects.id_io_state = io_states.id  left join users on io_objects.author = users.id  left join io_categories on io_objects.id_io_category = io_categories.id  left join io_types on io_objects.id_io_type = io_types.id  where 1 = 1  and  ( io_objects.""id"" in (select av.id_io_object from attributes a inner join attrs_categories ac on (ac.id_io_attribute = a.id and a.id = 237 ) inner join attrs_values av on ( av.value = '22233a' and av.id_attr_category = ac.id) ) or io_objects.""name"" is not null or io_objects.""id"" in (select av.id_io_object from attributes a inner join attrs_categories ac on (ac.id_io_attribute = a.id and a.id = 25 ) inner join attrs_values av on ( av.value = 'sss' and av.id_attr_category = ac.id) ) )  order by io_objects.id

explain analyze 
select 
     distinct io_objects.id, 
     io_objects.ref_table_name, 
     case when io_objects.id_search_template is null then NULL else search_templates.name end as id_search_template, 
     io_objects.record_text_color, 
     io_objects.record_fill_color, 
     io_objects.is_global, 
     organization.name as id_owner_org, 
     io_sync_types.name as id_sync_type, 
     maclabels.name as id_maclabel, 
     io_states.name as id_io_state, 
     io_objects.insert_time, 
     io_objects.is_system, 
     io_objects.information, 
     io_objects.table_name, 
     case when io_objects.author is null then NULL else users.fio end as author, 
     io_categories.name as id_io_category, 
     io_types.name as id_io_type, 
     io_objects.description, 
     io_objects.name 
 from 
     io_objects  
     left join search_templates on io_objects.id_search_template = search_templates.id  
     left join organization on io_objects.id_owner_org = organization.id  
     left join io_sync_types on io_objects.id_sync_type = io_sync_types.id  
     left join maclabels on io_objects.id_maclabel = maclabels.id  
     left join io_states on io_objects.id_io_state = io_states.id  
     left join users on io_objects.author = users.id  
     left join io_categories on io_objects.id_io_category = io_categories.id  
     left join io_types on io_objects.id_io_type = io_types.id  
     --inner join attrs_values av

 where 

1=1 and io_objects.id in 
(
select tt.id_io_object from (
select distinct t.id_io_object , t.a_2030, t.a_2037, t.a_2038 from 
    (
     SELECT *
     FROM crosstab(
           'select i.id_io_object , i.id_attr_category, i.value
            from attrs_values i
            where id_attr_category in (2030, 2037, 2038) 
            order by 1,2'::text, 
            'select unnest(ARRAY[2030, 2037, 2038]) order by 1'::text)
     AS ct(id_io_object int4, a_2030 varchar, a_2037 varchar, a_2038 varchar)
    ) as t

 where 
     (t.a_2038 = 'zxczxc')
     or (t.a_2038 = '22233' and t.a_2030 = 'sss')
     or (t.a_2037 = '2' and t.a_2038 = '2')
) as tt

)
        
 order by io_objects.id





     