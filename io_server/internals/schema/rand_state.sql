create table rand_state (id serial not null primary key,
                         state_rand bytea,
                         rand_seed bigint);
