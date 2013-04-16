
begin;
\encoding win1251
set search_path = public, pg_catalog;

\i ./functions/tasks/zarya22/attcodetotag.sql
\i ./functions/tasks/zarya22/tagtoattcode.sql

\i ./functions/tasks/zarya22/geturltype.sql
\i ./functions/tasks/zarya22/getwurltype.sql
\i ./functions/tasks/zarya22/fdoctoxml.sql
\i ./functions/tasks/zarya22/ufdoctoxml.sql
\i ./functions/tasks/zarya22/createpassport.sql
\i ./functions/tasks/zarya22/iotoxml.sql
\i ./functions/tasks/zarya22/readiofiles.sql

\i ./functions/tasks/zarya22/iofromxml.sql
\i ./functions/tasks/zarya22/iofromxmlcmd.sql
\i ./functions/tasks/zarya22/iofromxmlfdoc.sql
\i ./functions/tasks/zarya22/iofromxmlufdoc.sql
\i ./functions/tasks/zarya22/writeiofiles.sql

--\i ./functions/tasks/zarya22/msc_xml.sql
\i ./functions/tasks/zarya22/xmlinsertiorec.sql

\i ./functions/tasks/zarya22/trgioxml.sql

\i ./functions/tasks/zarya22/receiptfromxml.sql
\i ./functions/tasks/zarya22/receipttoxml.sql

\i ./functions/tasks/zarya22/getxmlparams.sql
\i ./functions/tasks/zarya22/getxmlvalue.sql
\i ./functions/tasks/zarya22/getxmlattrtype.sql
\i ./functions/tasks/zarya22/xmltext.sql
\i ./functions/tasks/zarya22/utilsfunc.sql
\i ./functions/tasks/zarya22/rectoxml.sql


commit;
