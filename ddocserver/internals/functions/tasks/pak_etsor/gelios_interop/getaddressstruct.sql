
select f_safe_drop_type('h_get_address_struct');
create type h_get_address_struct as(id int8, 
                                    region varchar, 
                                    auto varchar, 
                                    area varchar, 
                                    city varchar, 
                                    ctar varchar, 
                                    place varchar, 
                                    street varchar, 
                                    extr varchar, 
                                    sext varchar, 
                                    house varchar
                                    );



create or replace function getAddressStruct(int8) returns setof h_get_address_struct as
$BODY$
declare
    lastId alias for $1;
    r h_get_address_struct%rowtype;
    q1 text;
    q2 text;
begin

    q1 = 'select 1, i.aolevel, i.offname from getAddressItemTree(' || lastId || ') i order by 2'::text;
    q2 = 'select m FROM generate_series(1, 10) m'::text;

    for r in 
        select distinct t.id, 
           t.region, t.auto, t.area, t.city, t.ctar, t.place, t.street, t.extrcode, t.sextcode, t.house from 
        (
            SELECT *
            FROM crosstab(q1, q2)
            AS ct( id int8, 
                   "region" varchar, "auto" varchar, "area" varchar, "city" varchar, "ctar" varchar, "place" varchar, "street" varchar, "extrcode" varchar, "sextcode" varchar, "house" varchar)
        ) as t
        limit 1
    loop
        return next r;
    end loop;

    return;

end
$BODY$
language 'plpgsql' security definer;


--select * from tbl_eio_table_391
--select * from io_objects where id = 391
--select * from io_categories where id = 367
--select a.id, a.code, a.name from attrs_categories ac, attributes a where a.id = ac.id_io_attribute and ac.id_io_category = 366
--select * from getAddressItemTree(579);
--select array_to_string(array_agg(offname), ',') from getAddressItemTree(579);
--select * from getAddressStruct(579);

select f_safe_drop_type('h_get_address_item_tree');
create type h_get_address_item_tree as(id int8, 
                                       offname varchar, 
                                       formalname varchar, 
                                       parent int8, 
                                       regioncode varchar, 
                                       autocode varchar, 
                                       areacode varchar, 
                                       citycode varchar, 
                                       ctarcode varchar, 
                                       placecode varchar, 
                                       streetcode varchar, 
                                       extrcode varchar, 
                                       sextcode varchar, 
                                       aolevel int4, 
                                       ii_rec_order int4);

create or replace function getAddressItemTree(int8) returns setof h_get_address_item_tree as
$BODY$
declare
    lastId alias for $1;
    r h_get_address_item_tree%rowtype;

begin

    if(lastId isnull) then
        return;
    end if;

    for r in
        with recursive address_rec (id, "offname", "formalname", "parent", regioncode, autocode, areacode, citycode, ctarcode, placecode, streetcode, extrcode, sextcode, aolevel, ii_rec_order) 
        as 
        (
            select distinct 
                tbl_eio_table_391.id, 
                tbl_eio_table_391."offname", 
                tbl_eio_table_391."formalname", 
                tbl_eio_table_391."parent", 
                tbl_eio_table_391.regioncode,
                tbl_eio_table_391.autocode,
                tbl_eio_table_391.areacode,
                tbl_eio_table_391.citycode,
                tbl_eio_table_391.ctarcode,
                tbl_eio_table_391.placecode,
                tbl_eio_table_391.streetcode,
                tbl_eio_table_391.extrcode,
                tbl_eio_table_391.sextcode,
                tbl_eio_table_391.aolevel,
                0 
            from tbl_eio_table_391 
            where 
                tbl_eio_table_391.id = lastId

            UNION ALL 

            select 
                tbl_eio_table_391.id, 
                tbl_eio_table_391."offname", 
                tbl_eio_table_391."formalname", 
                tbl_eio_table_391."parent", 
                tbl_eio_table_391.regioncode,
                tbl_eio_table_391.autocode,
                tbl_eio_table_391.areacode,
                tbl_eio_table_391.citycode,
                tbl_eio_table_391.ctarcode,
                tbl_eio_table_391.placecode,
                tbl_eio_table_391.streetcode,
                tbl_eio_table_391.extrcode,
                tbl_eio_table_391.sextcode,
                tbl_eio_table_391.aolevel,
                address_rec.ii_rec_order+1 
            from 
                address_rec, tbl_eio_table_391
            where 
                address_rec.parent = tbl_eio_table_391."id"
        )

        select distinct 
                address_rec.id, 
                address_rec."offname", 
                address_rec."formalname", 
                address_rec."parent", 
                address_rec.regioncode,
                address_rec.autocode,
                address_rec.areacode,
                address_rec.citycode,
                address_rec.ctarcode,
                address_rec.placecode,
                address_rec.streetcode,
                address_rec.extrcode,
                address_rec.sextcode,
                address_rec.aolevel,
                address_rec.ii_rec_order 
        from 
            address_rec
        where 
            regioncode is not null 
            and (regioncode <> '00' or (regioncode = '00' and aolevel > 0))
        order by 
            address_rec.ii_rec_order desc
    loop
        return next r;
    end loop;

    return;
end
$BODY$
language 'plpgsql';



/*

select distinct t.id, 
        t.region, t.auto, t.area, t.city, t.ctar, t.place, t.street, t.extrcode, t.sextcode, t.house from 
    (
     SELECT *
     FROM crosstab(
           'select 1, --i.id, 
                   i.aolevel, i.offname
            from getAddressItemTree(579) i order by 2'::text,
            'select m FROM generate_series(1, 10) m'::text
            )
     AS ct( id int8, 
           "region" varchar, "auto" varchar, "area" varchar, "city" varchar, "ctar" varchar, "place" varchar, "street" varchar, "extrcode" varchar, "sextcode" varchar, "house" varchar)
    ) as t

 where 
     (t.a_2038 = 'zxczxc')
     or (t.a_2038 = '22233' and t.a_2030 = 'sss')
     or (t.a_2037 = '2' and t.a_2038 = '2')


SELECT * FROM crosstab(
  $$ SELECT year, month, qty FROM sales ORDER BY 1 $$,
  $$ SELECT m FROM generate_series(1,12) m $$
) AS (
  year int, "Jan" int, "Feb" int, "Mar" int, "Apr" int, "May" int, "Jun" int, "Jul" int, "Aug" int, "Sep" int, "Oct" int, "Nov" int, "Dec" int
);


*/