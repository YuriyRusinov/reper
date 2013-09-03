\i ./functions/io_objects/getrealtablename.sql

\i ./functions/io_objects/iodelete.sql

\i ./functions/io_objects/updateusertemplate.sql
\i ./functions/io_objects/is_numeric.sql

\i ./functions/io_objects/iogetusedattrs.sql

\i ./functions/io_objects/iogetlog.sql

\i ./functions/io_objects/iogetfiles.sql
\i ./functions/io_objects/ioinserturl.sql


\i ./functions/io_objects/iogetobject.sql

\i ./functions/io_objects/iogetobjectattrs.sql 
--должен идти всегда после iogetobjectattrs.sql
\i ./functions/io_objects/getioattrvaluehistory.sql
\i ./functions/io_objects/getioattrvalue.sql
\i ./functions/io_objects/ioinsert.sql
\i ./functions/io_objects/ioupdate.sql
\i ./functions/io_objects/ioinsertattr.sql
\i ./functions/io_objects/ioupdateattr.sql
\i ./functions/io_objects/ioinserttable.sql
\i ./functions/io_objects/ioupdatetable.sql

\i ./functions/io_objects/matchattrvalue.sql

\i ./functions/io_objects/iosetcompleted.sql

\i ./functions/io_objects/checkioforowner.sql
\i ./functions/io_objects/checkforexist.sql


\i ./functions/io_objects/ioinsertorgs.sql
\i ./functions/io_objects/iogetattrvalue.sql

\i ./functions/io_objects/ioinsertemptyrecord.sql

\i ./functions/io_objects/columnstoattrs.sql
\i ./functions/io_objects/translatetablestoschema.sql


--\i ./functions/io_objects/isapplicable.sql

-------------------
--Do NOT uncomment!
--This script is invoked in <ROOT>/internals/initiotriggers.sql file
-------------------
--\i ./functions/io_objects/triggers/readd_triggers.sql

\i ./functions/io_objects/triggers/logging/readd_logging.sql
