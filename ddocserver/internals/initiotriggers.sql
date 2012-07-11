\encoding win1251

begin;
\i ./functions/io_objects/triggers/readd_triggers.sql

\i ./functions/categories/triggers/readd_triggers.sql

\i ./functions/inf_exchange/createtriggers.sql

--\i ./functions/inf_exchange/checkcatforglobal.sql

commit;
