create table hist_processing_variants_streams (
    id_params_hist int4 not null,
    id_variant int4 not null,
    constraint PK_PROCESSING_HIST_VARIANTS primary key (id_params_hist, id_variant)
);

alter table hist_processing_variants_streams add constraint "fk_params_hist" FOREIGN KEY (id_params_hist) references histogram_params_streams(id) on update restrict on delete restrict;

alter table hist_processing_variants_streams add constraint "fk_ref_variant" FOREIGN KEY (id_variant) REFERENCES processing_variant(id);

create or replace function updateHistVariant() returns void as
$BODY$
declare
    id_hist int4;
    id_v int4[];

    r record;
    i int4;
begin
    for r in
        select id, variants from histogram_params_streams where variants is not null
    loop
        id_v = r.id_variants;
        for i in 1..array_upper (id_v, 1)
        loop
            insert into hist_processing_variants_streams (id_params_hist, id_variant) values (r.id, id_v[i]);
        end loop;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

select updateHistVariant();

alter table histogram_params_streams drop column variants;
