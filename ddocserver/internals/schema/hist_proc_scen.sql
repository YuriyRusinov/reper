create table hist_processing_scenario_streams (
    id_params_hist int4 not null,
    id_scenario int4 not null,
    constraint PK_PROCESSING_HIST_SCENARIO primary key (id_params_hist, id_scenario)
);

alter table hist_processing_scenario_streams add constraint "fk_params_hist" FOREIGN KEY (id_params_hist) references histogram_params_streams(id) on update restrict on delete restrict;

alter table hist_processing_scenario_streams add constraint "fk_ref_scenario" FOREIGN KEY (id_scenario) references processing_scenario(id);

alter table histogram_params_streams drop column scenarios;
