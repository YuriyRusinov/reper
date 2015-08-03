create table hist_processing_scenario_streams (
    id_params_hist int4 not null,
    id_scenario int4 not null,
    constraint PK_PROCESSING_HIST_SCENARIO primary key (id_params_hist, id_scenario)
);

alter table hist_processing_scenario_streams add constraint "fk_params_hist" FOREIGN KEY (id_params_hist) references histogram_params_streams(id) on update restrict on delete restrict;

alter table hist_processing_scenario_streams add constraint "fk_ref_scenario" FOREIGN KEY (id_scenario) references processing_scenario(id);
create or replace function updateHistScenario() returns void as
$BODY$
declare
    id_hist int4;
    id_sc int4[];

    r record;
    i int4;
begin
    for r in
        select id, scenarios from histogram_params_streams where scenarios is not null
    loop
        id_sc = r.id_scenarios;
        for i in 1..array_upper (id_sc, 1)
        loop
            insert into hist_processing_scenario_streams (id_params_hist, id_scenario) values (r.id, id_sc[i]);
        end loop;
    end loop;

    return;
end
$BODY$
language 'plpgsql';

select updateHistScenario();

alter table histogram_params_streams drop column scenarios;
